
Debian
====================
This directory contains files used to package scrytd/scryt-qt
for Debian-based Linux systems. If you compile scrytd/scryt-qt yourself, there are some useful files here.

## scryt: URI support ##


scryt-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install scryt-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your scryt-qt binary to `/usr/bin`
and the `../../share/pixmaps/scryt128.png` to `/usr/share/pixmaps`

scryt-qt.protocol (KDE)

