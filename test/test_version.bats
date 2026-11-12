#!/usr/bin/env bats

setup() {
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

@test "print version (short opts)" {
    run "$ZIPSIGN" -V
    [ "$status" -eq 0 ]
    output="${output%$'\r'}"
    [ "$output" = "1.7.0" ]
}

@test "print version (long opts)" {
    run "$ZIPSIGN" --version
    [ "$status" -eq 0 ]
    output="${output%$'\r'}"
    [ "$output" = "1.7.0" ]
}

