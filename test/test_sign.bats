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

assert_sign_help() {
    local text
    text=$(printf '%s\n' "$1" | tr -d '\r')

    [[ "$text" =~ zipsign[[:space:]]+v[0-9]+\.[0-9]+\.[0-9]+,[[:space:]]+Copyright[[:space:]]+\(c\)[[:space:]]+[0-9]{4}(-[0-9]{4})?[[:space:]]+Falk[[:space:]]+Werner ]]
    [[ "$text" =~ Signs[[:space:]]+and[[:space:]]+verifies[[:space:]]+ZIP[[:space:]]+archives ]]
    [[ "$text" =~ sign:[[:space:]]+Signs[[:space:]]+a[[:space:]]+zip[[:space:]]+archive\. ]]
    [[ "$text" =~ Usage: ]]
    [[ "$text" =~ zipsign[[:space:]]+sign[[:space:]]+-f[[:space:]]+\<value\>[[:space:]]+-p[[:space:]]+\<value\>[[:space:]]+-c[[:space:]]+\<value\> ]]
    [[ "$text" =~ -f,[[:space:]]+--file ]]
    [[ "$text" =~ -p,[[:space:]]+--private-key ]]
    [[ "$text" =~ -c,[[:space:]]+--certificate ]]
    [[ "$text" =~ -i,[[:space:]]+--intermediate ]]
    [[ "$text" =~ -e,[[:space:]]+--embed-certificate ]]
    [[ "$text" =~ -v,[[:space:]]+--verbose ]]
    [[ "$text" =~ -h,[[:space:]]+--help ]]
}

# ==============================================================================
# Help Tests
# ==============================================================================

@test "print sign help (short opt -h)" {
    run "$ZIPSIGN" sign -h
    [ "$status" -eq 0 ]
    assert_sign_help "$output"
}

@test "print sign help (long opt --help)" {
    run "$ZIPSIGN" sign --help
    [ "$status" -eq 0 ]
    assert_sign_help "$output"
}

# ==============================================================================
# Basic Signing Tests
# ==============================================================================

@test "sign zip archive and verify with zip tools and openssl" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -certfile "$CERT_FILE" \
        -CAfile "$CERT_FILE" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}

@test "sign zip archive using long options and verify" {
    run "$ZIPSIGN" sign --file "$ARCHIVE_FILE" --private-key "$KEY_FILE" --certificate "$CERT_FILE"
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -certfile "$CERT_FILE" \
        -CAfile "$CERT_FILE" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}

@test "sign zip archive with verbose output (short opt -v)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE" -v
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -certfile "$CERT_FILE" \
        -CAfile "$CERT_FILE" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}

@test "sign zip archive with verbose output (long opt --verbose)" {
    run "$ZIPSIGN" sign --file "$ARCHIVE_FILE" --private-key "$KEY_FILE" --certificate "$CERT_FILE" --verbose
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -certfile "$CERT_FILE" \
        -CAfile "$CERT_FILE" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}

# ==============================================================================
# Intermediate Certificate Tests
# ==============================================================================

@test "sign with intermediate certificate and verify against root CA (short opt)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -i "$BATS_TEST_TMPDIR/inter.crt"
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    # Verify using openssl: intermediate certificate is retrieved from the embedded CMS structure
    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -certfile "$BATS_TEST_TMPDIR/leaf.crt" \
        -CAfile "$BATS_TEST_TMPDIR/root.crt" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]

    # Also verify that intermediate certificate is present in CMS certificate list
    run openssl pkcs7 -inform DER -in "$BATS_TEST_TMPDIR/signature.der" -print_certs
    [ "$status" -eq 0 ]
    [[ "$output" =~ "inter CA" ]]

    # Verify using zipsign verify against root CA keyring
    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -k "$BATS_TEST_TMPDIR/root.crt"
    [ "$status" -eq 0 ]
}

@test "sign with intermediate certificate using long option (--intermediate)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    run "$ZIPSIGN" sign \
        --file "$ARCHIVE_FILE" \
        --private-key "$BATS_TEST_TMPDIR/leaf.key" \
        --certificate "$BATS_TEST_TMPDIR/leaf.crt" \
        --intermediate "$BATS_TEST_TMPDIR/inter.crt"
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -certfile "$BATS_TEST_TMPDIR/leaf.crt" \
        -CAfile "$BATS_TEST_TMPDIR/root.crt" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}

@test "fail signing when intermediate certificate file does not exist (negative)" {
    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" \
        -c "$CERT_FILE" \
        -i "$BATS_TEST_TMPDIR/nonexistent_intermediate.crt"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "file not found" ]]
}

@test "fail signing when intermediate certificate file is invalid (negative)" {
    echo "THIS_IS_NOT_A_VALID_CERTIFICATE" > "$BATS_TEST_TMPDIR/corrupt.crt"
    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" \
        -c "$CERT_FILE" \
        -i "$BATS_TEST_TMPDIR/corrupt.crt"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "failed to parse certificate" ]]
}

@test "verification fails when intermediate certificate was omitted during signing (negative)" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    # Sign WITHOUT providing the intermediate certificate
    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" \
        -c "$BATS_TEST_TMPDIR/leaf.crt"
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    # Verifying against root CA fails because the chain is incomplete
    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -certfile "$BATS_TEST_TMPDIR/leaf.crt" \
        -CAfile "$BATS_TEST_TMPDIR/root.crt"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "unable to get local issuer certificate" || "$output" =~ "certificate verify error" ]]

    # zipsign verify should also fail
    run "$ZIPSIGN" verify \
        -f "$ARCHIVE_FILE" \
        -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -k "$BATS_TEST_TMPDIR/root.crt"
    [ "$status" -ne 0 ]
}



# ==============================================================================
# Embedded Signer Certificate Tests (-e / --embed-certificate)
# ==============================================================================

@test "sign with embedded certificate (-e) allows verification without external certfile" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE" -e
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    # Verification succeeds without -certfile because signer certificate is embedded in CMS
    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -CAfile "$CERT_FILE" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]

    # Check that signer certificate is present in CMS certificate list
    run openssl pkcs7 -inform DER -in "$BATS_TEST_TMPDIR/signature.der" -print_certs
    [ "$status" -eq 0 ]
    [[ "$output" =~ "BEGIN CERTIFICATE" ]]
}

@test "sign with embedded certificate using long option (--embed-certificate)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE" --embed-certificate
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -CAfile "$CERT_FILE" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}

@test "verification without external certfile fails when certificate was not embedded (negative)" {
    # Default sign does NOT embed the certificate (CMS_NOCERTS flag is used)
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    # Verifying without -certfile must fail because the certificate is not inside CMS
    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -CAfile "$CERT_FILE"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "signer certificate not found" ]]

    # pkcs7 -print_certs should not contain any certificates
    run openssl pkcs7 -inform DER -in "$BATS_TEST_TMPDIR/signature.der" -print_certs
    [ "$status" -eq 0 ]
    [[ ! "$output" =~ "BEGIN CERTIFICATE" ]]
}


# ==============================================================================
# Multiple Signers Tests
# ==============================================================================

@test "sign with multiple signers and embedded certificates and verify each signer" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$KEY2" -c "$CERT2" \
        -e
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    # Combine both certificates into CAfile for openssl cms -verify
    cat "$CERT_FILE" "$CERT2" > "$BATS_TEST_TMPDIR/all_certs.pem"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -CAfile "$BATS_TEST_TMPDIR/all_certs.pem" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]

    # Verify with zipsign: against signer 1
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" --self-signed
    [ "$status" -eq 0 ]

    # Verify with zipsign: against signer 2
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT2" --self-signed
    [ "$status" -eq 0 ]

    # Verify with zipsign: against both signers
    run "$ZIPSIGN" verify -f "$ARCHIVE_FILE" -c "$CERT_FILE" -c "$CERT2" --self-signed
    [ "$status" -eq 0 ]
}

@test "sign with multiple signers using long options (--private-key and --certificate)" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign \
        --file "$ARCHIVE_FILE" \
        --private-key "$KEY_FILE" --certificate "$CERT_FILE" \
        --private-key "$KEY2" --certificate "$CERT2" \
        --embed-certificate
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    cat "$CERT_FILE" "$CERT2" > "$BATS_TEST_TMPDIR/all_certs.pem"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -CAfile "$BATS_TEST_TMPDIR/all_certs.pem" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}

@test "sign with multiple signers and intermediate certificate" {
    create_ca_chain "$BATS_TEST_TMPDIR" "root" "inter" "leaf"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$BATS_TEST_TMPDIR/leaf.key" -c "$BATS_TEST_TMPDIR/leaf.crt" \
        -i "$BATS_TEST_TMPDIR/inter.crt" \
        -e
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    cat "$CERT_FILE" "$BATS_TEST_TMPDIR/root.crt" > "$BATS_TEST_TMPDIR/trust_store.pem"

    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -CAfile "$BATS_TEST_TMPDIR/trust_store.pem" \
        -out "$BATS_TEST_TMPDIR/verified.bin"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "CMS Verification successful" ]]
}


@test "fail signing when key count exceeds certificate count (negative)" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$KEY2"
    [ "$status" -eq 1 ]
    [[ "$output" =~ "count of keys and signer certificated does not match" ]]
    assert_sign_help "$output"
}

@test "fail signing when certificate count exceeds key count (negative)" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -c "$CERT2"
    [ "$status" -eq 1 ]
    [[ "$output" =~ "count of keys and signer certificated does not match" ]]
    assert_sign_help "$output"
}

@test "fail signing when private key does not match certificate (negative)" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    # Pair KEY_FILE (from cert 1) with CERT2
    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" \
        -c "$CERT2"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "key values mismatch" || "$output" =~ "private key does not match certificate" ]]
}

@test "verification of multi-signed archive fails when trusted CA does not include signers (negative)" {
    KEY2="$BATS_TEST_TMPDIR/key2.pem"
    CERT2="$BATS_TEST_TMPDIR/cert2.pem"
    create_self_signed_cert "$KEY2" "$CERT2"

    KEY3="$BATS_TEST_TMPDIR/key3.pem"
    CERT3="$BATS_TEST_TMPDIR/cert3.pem"
    create_self_signed_cert "$KEY3" "$CERT3"

    run "$ZIPSIGN" sign \
        -f "$ARCHIVE_FILE" \
        -p "$KEY_FILE" -c "$CERT_FILE" \
        -p "$KEY2" -c "$CERT2" \
        -e
    [ "$status" -eq 0 ]

    extract_cms_signature "$ARCHIVE_FILE" "$BATS_TEST_TMPDIR/signature.der" "$BATS_TEST_TMPDIR/signed_data.bin"

    # Verification using unrelated cert3 as CAfile must fail because the signers are not trusted
    run openssl cms -verify \
        -binary \
        -inform DER \
        -in "$BATS_TEST_TMPDIR/signature.der" \
        -content "$BATS_TEST_TMPDIR/signed_data.bin" \
        -CAfile "$CERT3"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "CMS Verification failure" ]]
}

# ==============================================================================
# General CLI Negative Tests
# ==============================================================================

@test "fail signing when -f option is missing (negative)" {
    run "$ZIPSIGN" sign -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 1 ]
    [[ "$output" =~ "miss" && "$output" =~ "required argument: -f" ]]
    assert_sign_help "$output"
}

@test "fail signing when -p option is missing (negative)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -c "$CERT_FILE"
    [ "$status" -eq 1 ]
    [[ "$output" =~ "miss" && "$output" =~ "required argument: -p" ]]
    assert_sign_help "$output"
}

@test "fail signing when -c option is missing (negative)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE"
    [ "$status" -eq 1 ]
    [[ "$output" =~ "miss" && "$output" =~ "required argument: -c" ]]
    assert_sign_help "$output"
}

@test "fail signing when archive file does not exist (negative)" {
    run "$ZIPSIGN" sign -f "$BATS_TEST_TMPDIR/nonexistent.zip" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -ne 0 ]
}

@test "fail signing when private key file does not exist (negative)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$BATS_TEST_TMPDIR/nonexistent.key" -c "$CERT_FILE"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "file not found" ]]
}

@test "fail signing when certificate file does not exist (negative)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$BATS_TEST_TMPDIR/nonexistent.crt"
    [ "$status" -ne 0 ]
    [[ "$output" =~ "file not found" ]]
}

@test "fail signing with unrecognized option (negative)" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE" --unrecognized-option
    [ "$status" -eq 1 ]
    [[ "$output" =~ "unrecognized argument" ]]
    assert_sign_help "$output"
}

