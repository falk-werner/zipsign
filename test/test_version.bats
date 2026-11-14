#!/usr/bin/env bats

load 'test_helper.sh'

@test "print version (short opts)" {
    run "$ZIPSIGN" -V
    [ "$status" -eq 0 ]
    output="${output%$'\r'}"
    [ "$output" = "2.0.0" ]
}

@test "print version (long opts)" {
    run "$ZIPSIGN" --version
    [ "$status" -eq 0 ]
    output="${output%$'\r'}"
    [ "$output" = "2.0.0" ]
}

