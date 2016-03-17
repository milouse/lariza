#!/usr/bin/env bash

which wget &> /dev/null
[ "$?" -eq 1 ] \
    && echo "You need wget to run this script." && exit 1

which sed &> /dev/null
[ "$?" -eq 1 ] \
    && echo "You need sed to run this script." && exit 1

SOURCE_URL=${SOURCE_URL:-"https://raw.githubusercontent.com/StevenBlack/hosts/master/alternates/gambling-porn/hosts"}

MUST_INSTALL=0
[ -n "$1" ] && [ "$1" = "install" ] && MUST_INSTALL=1

wget -O adblock.black $SOURCE_URL

[ "$?" != '0' ] || [ ! -f adblock.black ] \
    && echo 'Something wrong happen while downloading source file.' && exit 1

sed -i -e '/^\s*#/d' \
    -e '/^127.0.0.1/d' \
    -e '/^255.255.255.255/d' \
    -e '/^::1 /d' \
    -e '/^fe80::1/d' \
    -e '/^$/d' \
    -e 's|^0.0.0.0 |^https?://|g' \
    -e 's/^\(.*\)$/\1.*/g' adblock.black

if [ "$MUST_INSTALL" -eq 1 ]; then
    echo 'Installing generated blocklist'
    [ ! -d ~/.config/lariza ] && mkdir -p ~/.config/lariza
    cp adblock.black ~/.config/lariza/
    rm adblock.black
fi
