#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

SCRYTD=${SCRYTD:-$BINDIR/scrytd}
SCRYTCLI=${SCRYTCLI:-$BINDIR/scryt-cli}
SCRYTTX=${SCRYTTX:-$BINDIR/scryt-tx}
SCRYTQT=${SCRYTQT:-$BINDIR/qt/scryt-qt}

[ ! -x $SCRYTD ] && echo "$SCRYTD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BTCVER=($($SCRYTCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for scrytd if --version-string is not set,
# but has different outcomes for scryt-qt and scryt-cli.
echo "[COPYRIGHT]" > footer.h2m
$SCRYTD --version | sed -n '1!p' >> footer.h2m

for cmd in $SCRYTD $SCRYTCLI $SCRYTTX $SCRYTQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
