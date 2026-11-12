#!/usr/bin/env bash

setup_zipsign() {
    if [ -n "$ZIPSIGN_BIN" ]; then
        ZIPSIGN="$ZIPSIGN_BIN"
    elif [ -x "build/zipsign" ]; then
        ZIPSIGN="build/zipsign"
    elif [ -x "build/zipsign.exe" ]; then
        ZIPSIGN="build/zipsign.exe"
    elif [ -x "./zipsign" ]; then
        ZIPSIGN="./zipsign"
    elif [ -x "./zipsign.exe" ]; then
        ZIPSIGN="./zipsign.exe"
    else
        ZIPSIGN="zipsign"
    fi
}

setup() {
    setup_zipsign
}
