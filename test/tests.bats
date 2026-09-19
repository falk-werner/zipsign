#!/usr/bin/env bats

setup() {
    ZIPSIGN=zipsign
    if [[ -e build/zipsign ]] ; then
        ZIPSIGN=build/zipsign
    fi
}

@test "dummy" {
    $ZIPSIGN -V
}