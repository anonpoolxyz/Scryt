Bash Completion
---------------------

The following script provide bash completion functionality for `sycoind` and `scryt-cli`.

* `contrib/scrytd.bash-completion`
* `contrib/scryt-cli.bash-completion`
* `contrib/_osx.bash-completion`

### OSX ###
Use `brew` to install `bash-completion` then update `~/.bashrc` to include the completion scripts and helper functions to provide `have()` and `_have()` on OSX.

The example assumes Scryt was compiled in `~/scryt` and the scripts will be availabe in `~/scryt/contrib`, however they can be moved to a different location.

```
brew install bash bash-completion
sudo bash -c 'echo /usr/local/bin/bash >> /etc/shells'
chsh -s /usr/local/bin/bash

BASHRC=$(cat <<EOF
if [ -f $(brew --prefix)/etc/bash_completion ]; then
  . $(brew --prefix)/etc/bash_completion
fi
. ~/scryt/contrib/_osx.bash-completion
. ~/scryt/contrib/scrytd.bash-completion
. ~/scryt/contrib/scryt-cli.bash-completion
EOF
)

grep -q "/etc/bash_completion" ~/.bashrc || echo "$BASHRC" >> ~/.bashrc
. ~/.bashrc

```