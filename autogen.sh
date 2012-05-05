#! /bin/sh
# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

if [ ! -d m4 ]; then
    mkdir m4
fi

autoreconf -i -I m4
