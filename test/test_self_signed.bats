#!/usr/bin/env bats

setup() {
    ZIPSIGN=zipsign
    if [[ -e build/zipsign ]] ; then
        ZIPSIGN=build/zipsign
    fi

    TESTDIR="$(mktemp -p /tmp -d zipsign_test_XXXXXX)"
}

teardown() {
    rm -rf "${TESTDIR}"
}

@test "sign and verify self signed (short opts)" {
    cp test-src/resources/test.zip "${TESTDIR}/test.zip"
    ${ZIPSIGN} sign -f "${TESTDIR}/test.zip" -c build/self-signed/cert.pem -p build/self-signed/key.pem
    ${ZIPSIGN} verify -f "${TESTDIR}/test.zip" -c build/self-signed/cert.pem -s
}

@test "sign and verify self signed (long opts)" {
    cp test-src/resources/test.zip "${TESTDIR}/test.zip"
    ${ZIPSIGN} sign --file "${TESTDIR}/test.zip" --certificate build/self-signed/cert.pem --private-key build/self-signed/key.pem
    ${ZIPSIGN} verify --file "${TESTDIR}/test.zip" --certificate build/self-signed/cert.pem --self-signed
}
