#!/usr/bin/env bats

setup() {
    ZIPSIGN=zipsign
    if [[ -e build/zipsign ]] ; then
        ZIPSIGN=build/zipsign
    fi
}


@test "print version (short opts)" {
    $ZIPSIGN -V
    VERSION="$($ZIPSIGN -V)"
    if [[ "1.7.0" != "$VERSION" ]] ; then
        false
    fi
}

@test "print version (long opts)" {
    $ZIPSIGN --version
    VERSION="$($ZIPSIGN --version)"
    if [[ "1.7.0" != "$VERSION" ]] ; then
        false
    fi
}
