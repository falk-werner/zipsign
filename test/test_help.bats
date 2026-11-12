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

assert_general_help() {
    local text
    text=$(printf '%s\n' "$1" | tr -d '\r')

    # Header / Copyright
    [[ "$text" =~ zipsign[[:space:]]+v[0-9]+\.[0-9]+\.[0-9]+,[[:space:]]+Copyright[[:space:]]+\(c\)[[:space:]]+[0-9]{4}(-[0-9]{4})?[[:space:]]+Falk[[:space:]]+Werner ]]
    [[ "$text" =~ Signs[[:space:]]+and[[:space:]]+verifies[[:space:]]+ZIP[[:space:]]+archives ]]

    # Usage
    [[ "$text" =~ Usage: ]]
    [[ "$text" =~ zipsign[[:space:]]+sign[[:space:]]+\<args\.\.\.\>[[:space:]]+\|[[:space:]]+verify[[:space:]]+\<args\.\.\.\>[[:space:]]+\|[[:space:]]+info[[:space:]]+\<args\.\.\.\>[[:space:]]+\|[[:space:]]+-h ]]

    # Verbs
    [[ "$text" =~ Verbs: ]]
    [[ "$text" =~ sign[[:space:]]+Signs[[:space:]]+a[[:space:]]+zip[[:space:]]+archive\. ]]
    [[ "$text" =~ verify[[:space:]]+Verifies[[:space:]]+the[[:space:]]+signature[[:space:]]+of[[:space:]]+a[[:space:]]+zip[[:space:]]+archive\. ]]
    [[ "$text" =~ info[[:space:]]+Print[[:space:]]+info[[:space:]]+about[[:space:]]+the[[:space:]]+signature[[:space:]]+of[[:space:]]+zip[[:space:]]+archive\. ]]
    [[ "$text" =~ -V,[[:space:]]+--version[[:space:]]+Print[[:space:]]+version\. ]]
    [[ "$text" =~ -h,[[:space:]]+--help[[:space:]]+Print[[:space:]]+usage\. ]]

    # Examples
    [[ "$text" =~ Examples: ]]
    [[ "$text" =~ Sign: ]]
    [[ "$text" =~ zipsign[[:space:]]+sign[[:space:]]+-f[[:space:]]+archive\.zip[[:space:]]+-p[[:space:]]+key\.pem[[:space:]]+-c[[:space:]]+cert\.pem ]]
    [[ "$text" =~ Verify: ]]
    [[ "$text" =~ zipsign[[:space:]]+verify[[:space:]]+-f[[:space:]]+archive\.zip[[:space:]]+-c[[:space:]]+cert\.pem ]]
    [[ "$text" =~ zipsign[[:space:]]+verify[[:space:]]+-f[[:space:]]+archive\.zip[[:space:]]+-c[[:space:]]+cert\.pem[[:space:]]+--self-signed ]]
}

@test "print general help (short opts)" {
    run "$ZIPSIGN" -h
    [ "$status" -eq 0 ]
    assert_general_help "$output"
}

@test "print general help (long opts)" {
    run "$ZIPSIGN" --help
    [ "$status" -eq 0 ]
    assert_general_help "$output"
}
