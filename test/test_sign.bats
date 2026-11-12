#!/usr/bin/env bats

load 'test_helper.sh'

setup() {
    setup_zipsign
    KEY_FILE="$BATS_TEST_TMPDIR/key.pem"
    CERT_FILE="$BATS_TEST_TMPDIR/cert.pem"
    ARCHIVE_FILE="$BATS_TEST_TMPDIR/archive.zip"

    # Generate key and self-signed certificate for signing (-batch uses default openssl values without prompting)
    openssl req -x509 -newkey rsa:2048 -nodes \
        -keyout "$KEY_FILE" \
        -out "$CERT_FILE" \
        -days 1 \
        -batch

    # Create a test zip archive
    echo "Test content for zipsign verification" > "$BATS_TEST_TMPDIR/sample.txt"
    (cd "$BATS_TEST_TMPDIR" && zip -q archive.zip sample.txt)
}

@test "sign zip archive and verify with zip tools and openssl" {
    run "$ZIPSIGN" sign -f "$ARCHIVE_FILE" -p "$KEY_FILE" -c "$CERT_FILE"
    [ "$status" -eq 0 ]

    # Extract comment from zip archive using zipnote (part of zip tools) or unzip -z
    if command -v zipnote >/dev/null 2>&1; then
        comment=$(zipnote "$ARCHIVE_FILE" | awk '/^@ \(zip file comment below this line\)$/{getline; print}' | tr -d '\r')
    else
        comment=$(unzip -z "$ARCHIVE_FILE" | sed -n '1!p' | tr -d '\r')
    fi

    # Verify signature prefix and extract base64 data
    prefix="ZipSign=data:application/cms;base64,"
    [[ "$comment" == "$prefix"* ]]
    b64_sig="${comment#"$prefix"}"

    # Decode base64 signature to DER
    printf '%s' "$b64_sig" | openssl base64 -d -A -out "$BATS_TEST_TMPDIR/signature.der"

    # Extract signed data from zip archive (everything before 2-byte comment length field)
    file_size=$(wc -c < "$ARCHIVE_FILE" | tr -d '[:space:]')
    signed_len=$(( file_size - ${#comment} - 2 ))
    head -c "$signed_len" "$ARCHIVE_FILE" > "$BATS_TEST_TMPDIR/signed_data.bin"

    # Verify CMS detached signature using openssl
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

