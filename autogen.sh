#! /bin/sh
# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

AUTORECONF_ARGS=-i
if [ -d m4 ]; then
    AUTORECONF_ARGS="$AUTORECONF_ARGS -I m4"
fi

autoreconf $AUTORECONF_ARGS
