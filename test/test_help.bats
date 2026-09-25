#!/usr/bin/env bats

setup() {
    ZIPSIGN=zipsign
    if [[ -e build/zipsign ]] ; then
        ZIPSIGN=build/zipsign
    fi
}


@test "print help (short opts)" {
    $ZIPSIGN -h
    HELP="$($ZIPSIGN -h)"

    # Help container copyright
    echo "${HELP}" | grep 'zipsign .*, Copyright (c) .* Falk Werner'

    # Help contains description
    echo "${HELP}" | grep 'Signs and verifies ZIP archives'

    # Help containes usage
    echo "${HELP}" | grep 'Usage:'

    # Help containes verbs
    echo "${HELP}" | grep 'Verbs:'
    echo "${HELP}" | grep '.* sign .* Signs a zip archive.'
    echo "${HELP}" | grep '.* verify .* Verifies the signature of a zip archive.'
    echo "${HELP}" | grep '.* info .* Print info about the signature of zip archive.'
    echo "${HELP}" | grep '.* -V, --version .* Print version.'
    echo "${HELP}" | grep '.* -h, --help .* Print usage.'

    # Help contains some examples
    echo "${HELP}" | grep 'Examples:'
    echo "${HELP}" | grep '.* zipsign sign'
    echo "${HELP}" | grep '.* zipsign verify'
}

@test "print help (long opts)" {
    $ZIPSIGN --help
    HELP="$($ZIPSIGN --help)"

    # Help container copyright
    echo "${HELP}" | grep 'zipsign .*, Copyright (c) .* Falk Werner'

    # Help contains description
    echo "${HELP}" | grep 'Signs and verifies ZIP archives'

    # Help containes usage
    echo "${HELP}" | grep 'Usage:'

    # Help containes verbs
    echo "${HELP}" | grep 'Verbs:'
    echo "${HELP}" | grep '.* sign .* Signs a zip archive.'
    echo "${HELP}" | grep '.* verify .* Verifies the signature of a zip archive.'
    echo "${HELP}" | grep '.* info .* Print info about the signature of zip archive.'
    echo "${HELP}" | grep '.* -V, --version .* Print version.'
    echo "${HELP}" | grep '.* -h, --help .* Print usage.'

    # Help contains some examples
    echo "${HELP}" | grep 'Examples:'
    echo "${HELP}" | grep '.* zipsign sign'
    echo "${HELP}" | grep '.* zipsign verify'
}
