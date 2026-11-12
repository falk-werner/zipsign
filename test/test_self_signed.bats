#!/usr/bin/env bats

setup() {
    ZIPSIGN=zipsign
    if [[ -e build/zipsign ]] ; then
        ZIPSIGN=build/zipsign
    fi

    TESTDIR="$(mktemp -p /tmp -d zipsign_test_XXXXXX)"
    openssl req -x509 -newkey rsa:4096 \
        -keyout "${TESTDIR}/key.pem" -out "${TESTDIR}/cert.pem" \
        -days 365 -nodes -batch
}

teardown() {
    rm -rf "${TESTDIR}"
}

@test "sign and verify self signed (short opts)" {
    cp test-src/resources/test.zip "${TESTDIR}/test.zip"
    ${ZIPSIGN} sign --file "${TESTDIR}/test.zip" --certificate "${TESTDIR}/cert.pem" --private-key "${TESTDIR}/key.pem"
    ${ZIPSIGN} verify --file "${TESTDIR}/test.zip" --certificate "${TESTDIR}/cert.pem" --self-signed
}

@test "sign and verify self signed (long opts)" {
    cp test-src/resources/test.zip "${TESTDIR}/test.zip"
    ${ZIPSIGN} sign --file "${TESTDIR}/test.zip" --certificate "${TESTDIR}/cert.pem" --private-key "${TESTDIR}/key.pem"
    ${ZIPSIGN} verify --file "${TESTDIR}/test.zip" --certificate "${TESTDIR}/cert.pem" --self-signed
}
