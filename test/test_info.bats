#!/usr/bin/env bats

load 'test_helper.sh'

setup() {
    setup_zipsign
    KEY_FILE="$BATS_TEST_TMPDIR/key.pem"
    CERT_FILE="$BATS_TEST_TMPDIR/cert.pem"
    ARCHIVE_FILE="$BATS_TEST_TMPDIR/archive.zip"

    # Generate key and self-signed certificate for signing
    create_self_signed_cert "$KEY_FILE" "$CERT_FILE"

    # Create a test zip archive
    echo "Test content for zipsign info" > "$BATS_TEST_TMPDIR/sample.txt"
    (cd "$BATS_TEST_TMPDIR" && zip -q archive.zip sample.txt)
}

assert_info_help() {
    local text
    text=$(printf '%s\n' "$1" | tr -d '\r')

    [[ "$text" =~ zipsign[[:space:]]+v[0-9]+\.[0-9]+\.[0-9]+,[[:space:]]+Copyright[[:space:]]+\(c\)[[:space:]]+[0-9]{4}(-[0-9]{4})?[[:space:]]+Falk[[:space:]]+Werner ]]
    [[ "$text" =~ Signs[[:space:]]+and[[:space:]]+verifies[[:space:]]+ZIP[[:space:]]+archives ]]
    [[ "$text" =~ info:[[:space:]]+Print[[:space:]]+info[[:space:]]+about[[:space:]]+the[[:space:]]+signature[[:space:]]+of[[:space:]]+zip[[:space:]]+archive\. ]]
    [[ "$text" =~ Usage: ]]
    [[ "$text" =~ zipsign[[:space:]]+info[[:space:]]+-f[[:space:]]+\<value\>[[:space:]]+\|[[:space:]]+-h ]]
    [[ "$text" =~ -f,[[:space:]]+--file ]]
    [[ "$text" =~ -h,[[:space:]]+--help ]]
}

# ==============================================================================
# Help Tests
# ==============================================================================

@test "print info help (short opt -h)" {
    run "$ZIPSIGN" info -h
    [ "$status" -eq 0 ]
    assert_info_help "$output"
}

@test "print info help (long opt --help)" {
    run "$ZIPSIGN" info --help
    [ "$status" -eq 0 ]
    assert_info_help "$output"
}

# ==============================================================================
# Basic Info Tests
# ==============================================================================

@test "print signature info of signed zip archive using short options" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" info -f "$ARCHIVE_FILE"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS_ContentInfo:" ]]
    [[ "$output" =~ "contentType: pkcs7-signedData" ]]
    [[ "$output" =~ "signerInfos:" ]]
    [[ "$output" =~ "certificates:"[[:space:]]+"<ABSENT>" ]]
}

@test "print signature info of signed zip archive using long options" {
    run "$ZIPSIGN" sign --file "$ARCHIVE_FILE" --private-key "$KEY_FILE" --certificate "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" info --file "$ARCHIVE_FILE"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS_ContentInfo:" ]]
    [[ "$output" =~ "contentType: pkcs7-signedData" ]]
    [[ "$output" =~ "signerInfos:" ]]
}

# ==============================================================================
# Embedded Certificate Tests
# ==============================================================================

@test "print signature info with embedded certificate (short opt -e)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE" -e
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" info -f "$ARCHIVE_FILE"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS_ContentInfo:" ]]
    [[ "$output" =~ "d.certificate:" ]]
    [[ ! "$output" =~ "certificates:"[[:space:]]+"<ABSENT>" ]]
}

@test "print signature info with embedded certificate using long option (--embed-certificate)" {
    run "$ZIPSIGN" sign --file "$ARCHIVE_FILE" --private-key "$KEY_FILE" --certificate "$CERT_FILE" --embed-certificate
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" info --file "$ARCHIVE_FILE"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS_ContentInfo:" ]]
    [[ "$output" =~ "d.certificate:" ]]
    [[ ! "$output" =~ "certificates:"[[:space:]]+"<ABSENT>" ]]
}

# ==============================================================================
# Intermediate Certificate Tests
# ==============================================================================

@test "print signature info with intermediate certificate in CMS structure" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -i "$BATS_TEST_TMPDIR/inter.crt"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" info -f "$ARCHIVE_FILE"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS_ContentInfo:" ]]
    [[ "$output" =~ "inter CA" ]]
}

# ==============================================================================
# Multi-Signer Tests
# ==============================================================================

@test "print signature info for archive signed by multiple signers" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$KEY2" -c "$CERT2"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" info -f "$ARCHIVE_FILE"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS_ContentInfo:" ]]

    count=$(grep -c "signerInfos:" <<< "$output" || true)
    [ "$count" -ge 1 ]
    sig_count=$(grep -c "signatureAlgorithm:" <<< "$output" || true)
    [ "$sig_count" -ge 2 ]
}

# ==============================================================================
# CLI Argument Validation & Negative Tests
# ==============================================================================

@test "fail info when -f option is missing (negative)" {
    run "$ZIPSIGN" info
    [ "$status" -eq 1 ]
    [[ "$output" =~ "required argument: -f" ]]
    assert_info_help "$output"
}

@test "fail info with unrecognized option (negative)" {
    run "$ZIPSIGN" info -f "$ARCHIVE_FILE" --unrecognized-option
    [ "$status" -eq 1 ]
    [[ "$output" =~ "unrecognized argument" ]]
    assert_info_help "$output"
}

@test "fail info when archive file does not exist (negative)" {
    run "$ZIPSIGN" info -f "$BATS_TEST_TMPDIR/nonexistent.zip"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "file not found" ]]
}

@test "fail info on unsigned archive (negative)" {
    run "$ZIPSIGN" info -f "$ARCHIVE_FILE"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "missing signature" ]]
}

@test "fail info on archive with non-zipsign zip comment (negative)" {
    echo "Regular zip comment not related to zipsign" | zip -q -z "$ARCHIVE_FILE"

    run "$ZIPSIGN" info -f "$ARCHIVE_FILE"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "missing signature" ]]
}

@test "fail info on archive with corrupted signature base64 (negative)" {
    printf "ZipSign=data:application/cms;base64,CORRUPTED_BASE64_DATA" | zip -q -z "$ARCHIVE_FILE"

    run "$ZIPSIGN" info -f "$ARCHIVE_FILE"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "base64" || "$output" =~ "error" ]]
}

