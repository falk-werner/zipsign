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
    echo "Test content for zipsign verification" > "$BATS_TEST_TMPDIR/sample.txt"
    (cd "$BATS_TEST_TMPDIR" && zip -q archive.zip sample.txt)
}

assert_verify_help() {
    local text
    text=$(printf '%s\n' "$1" | tr -d '\r')

    [[ "$text" =~ zipsign[[:space:]]+v[0-9]+\.[0-9]+\.[0-9]+,[[:space:]]+Copyright[[:space:]]+\(c\)[[:space:]]+[0-9]{4}(-[0-9]{4})?[[:space:]]+Falk[[:space:]]+Werner ]]
    [[ "$text" =~ Signs[[:space:]]+and[[:space:]]+verifies[[:space:]]+ZIP[[:space:]]+archives ]]
    [[ "$text" =~ verify:[[:space:]]+Verifies[[:space:]]+the[[:space:]]+signature[[:space:]]+of[[:space:]]+a[[:space:]]+zip[[:space:]]+archive\. ]]
    [[ "$text" =~ Usage: ]]
    [[ "$text" =~ zipsign[[:space:]]+verify[[:space:]]+-f[[:space:]]+\<value\>[[:space:]]+-c[[:space:]]+\<value\> ]]
    [[ "$text" =~ -f,[[:space:]]+--file ]]
    [[ "$text" =~ -c,[[:space:]]+--certificate ]]
    [[ "$text" =~ -k,[[:space:]]+--keyring ]]
    [[ "$text" =~ -v,[[:space:]]+--verbose ]]
    [[ "$text" =~ -s,[[:space:]]+--self-signed ]]
    [[ "$text" =~ -h,[[:space:]]+--help ]]
}

# ==============================================================================
# Basic Verification Tests
# ==============================================================================

@test "verify signed zip archive using short options" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

@test "verify signed zip archive using long options" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify --file "$ARCHIVE_FILE" --certificate "$CERT_FILE" --self-signed
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

@test "verify signed zip archive with verbose output (short opt -v)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s -v
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

@test "verify signed zip archive with verbose output (long opt --verbose)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify --file "$ARCHIVE_FILE" --certificate "$CERT_FILE" --self-signed --verbose
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

# ==============================================================================
# PKI & Keyring Verification Tests
# ==============================================================================

@test "verify signature against root CA keyring (short opt -k)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -i "$BATS_TEST_TMPDIR/inter.crt"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -k "$BATS_TEST_TMPDIR/root.crt"
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

@test "verify signature against root CA keyring using long options (--keyring)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    run "$ZIPSIGN" sign \
        --file "$ARCHIVE_FILE" \
        --private-key "$BATS_TEST_TMPDIR/leaf.key" \
        --certificate "$BATS_TEST_TMPDIR/leaf.crt" \
        --intermediate "$BATS_TEST_TMPDIR/inter.crt"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify \
        --file "$ARCHIVE_FILE" \
        --certificate "$BATS_TEST_TMPDIR/leaf.crt" \
        --keyring "$BATS_TEST_TMPDIR/root.crt"
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

@test "fail verification when keyring does not contain issuer CA (negative)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"
    create_self_signed_cert "$BATS_TEST_TMPDIR/other.key" "$BATS_TEST_TMPDIR/other.crt"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -i "$BATS_TEST_TMPDIR/inter.crt"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -k "$BATS_TEST_TMPDIR/other.crt"
    [ "$status" -eq 3 ]
    [[ "$output" =~ "INVALID_CERTIFICATE_CHAIN" ]]
}

@test "fail verification when intermediate certificate was omitted during signing (negative)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    # Sign without intermediate certificate
    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" \
        -c "$BATS_TEST_TMPDIR/leaf.crt"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -k "$BATS_TEST_TMPDIR/root.crt"
    [ "$status" -eq 3 ]
    [[ "$output" =~ "INVALID_CERTIFICATE_CHAIN" ]]
}

@test "fail verification of CA-issued certificate when keyring is omitted and not self-signed (negative)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -i "$BATS_TEST_TMPDIR/inter.crt"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$BATS_TEST_TMPDIR/leaf.crt"
    [ "$status" -eq 3 ]
    [[ "$output" =~ "INVALID_CERTIFICATE_CHAIN" ]]
}

# ==============================================================================
# Multiple Signers Tests
# ==============================================================================

@test "verify archive signed by multiple signers for each individual signer" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$KEY2" -c "$CERT2" \
        -e
    [ "$status" -eq 0 ]

    # Verify against signer 1
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]

    # Verify against signer 2
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT2" -s
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

@test "verify archive signed by multiple signers specifying all signers" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$KEY2" -c "$CERT2" \
        -e
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$CERT_FILE" \
        -c "$CERT2" \
        -s
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]
}

@test "fail verification when non-signer certificate is specified for multi-signed archive (negative)" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    KEY3="$BATS_TEST_TMPDIR/key3.pem"
    CERT3="$BATS_TEST_TMPDIR/cert3.pem"
    create_self_signed_cert "$KEY3" "$CERT3"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$KEY2" -c "$CERT2"
    [ "$status" -eq 0 ]

    # Verifying with non-signer CERT3 must fail
    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$CERT3" \
        -s
    [ "$status" -eq 3 ]
    [[ "$output" =~ "INVALID_CERTIFICATE_CHAIN" ]]
}

# ==============================================================================
# Tampering and Data Integrity Tests
# ==============================================================================

@test "fail verification when archive content is tampered with (negative)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    # Verify first that untouched archive is OK
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s
    [ "$status" -eq 0 ]
    [ "$output" = "OK" ]

    # Tamper with the zip content (flip a byte early in the archive payload)
    printf '\xFF' | dd of="$ARCHIVE_FILE" bs=1 seek=10 count=1 conv=notrunc 2>/dev/null

    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s
    [ "$status" -eq 4 ]
    [[ "$output" =~ "INVALID_SIGNATURE" ]]
}

@test "fail verification when verified against wrong certificate (negative)" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT2" -s
    [ "$status" -eq 3 ]
    [[ "$output" =~ "INVALID_CERTIFICATE_CHAIN" ]]
}


# ==============================================================================
# Missing or Malformed Signature Tests
# ==============================================================================

@test "fail verification on unsigned archive (negative)" {
    # ARCHIVE_FILE is created in setup() unsigned
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s
    [ "$status" -eq 2 ]
    [[ "$output" =~ "INVALID_MISSING_SIGNATURE" ]]
}

@test "fail verification on unsigned archive with verbose output (negative)" {
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s -v
    [ "$status" -eq 2 ]
    [[ "$output" =~ "INVALID_MISSING_SIGNATURE" ]]
    [[ "$output" =~ "error: missing signature" ]]
}

@test "fail verification on archive with non-zipsign zip comment (negative)" {
    echo "Regular zip comment not related to zipsign" | zip -q -z "$ARCHIVE_FILE"

    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s
    [ "$status" -eq 2 ]
    [[ "$output" =~ "INVALID_MISSING_SIGNATURE" ]]
}

@test "fail verification on archive with corrupted signature base64 (negative)" {
    printf "ZipSign=data:application/cms;base64,CORRUPTED_BASE64_DATA" | zip -q -z "$ARCHIVE_FILE"

    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -s
    [ "$status" -eq 1 ]
    [[ "$output" =~ "INVALID" ]]
}

# ==============================================================================
# Help Tests
# ==============================================================================

@test "print verify help (short opt -h)" {
    run "$ZIPSIGN" verify -h
    [ "$status" -eq 0 ]
    assert_verify_help "$output"
}

@test "print verify help (long opt --help)" {
    run "$ZIPSIGN" verify --help
    [ "$status" -eq 0 ]
    assert_verify_help "$output"
}

# ==============================================================================
# CLI Argument Validation Tests (Negative)
# ==============================================================================

@test "fail verification when -f option is missing (negative)" {
    run "$ZIPSIGN" verify -c "$CERT_FILE"
    [ "$status" -eq 1 ]
    [[ "$output" =~ "missing required argument: -f" ]]
}

@test "fail verification when -c option is missing (negative)" {
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE"
    [ "$status" -eq 1 ]
    [[ "$output" =~ "missing required argument: -c" ]]
}

@test "fail verification when archive file does not exist (negative)" {
    run "$ZIPSIGN" verify -f "$BATS_TEST_TMPDIR/nonexistent.zip" -c "$CERT_FILE"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "INVALID" ]]
}

@test "fail verification when certificate file does not exist (negative)" {
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$BATS_TEST_TMPDIR/nonexistent.crt"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "file not found" ]]
}

@test "fail verification when keyring file does not exist (negative)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -k "$BATS_TEST_TMPDIR/nonexistent.crt"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "INVALID" ]]
}

@test "fail verification with unrecognized option (negative)" {
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" --unrecognized-option
    [ "$status" -eq 1 ]
    [[ "$output" =~ "unrecognized argument" ]]
}

