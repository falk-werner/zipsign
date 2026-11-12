#!/usr/bin/env bash

setup_zipsign() {
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

setup() {
    setup_zipsign
}

create_self_signed_cert() {
    local key_file="$1"
    local cert_file="$2"
    openssl req -x509 -newkey rsa:2048 -nodes \
        -keyout "$key_file" \
        -out "$cert_file" \
        -days 1 \
        -batch 2>/dev/null
}

create_ca_chain() {
    local dir="$1"
    local root_name="${2:-root}"
    local inter_name="${3:-intermediate}"
    local leaf_name="${4:-leaf}"

    # Root CA
    cat <<EOF > "$dir/${root_name}.cnf"
[ req ]
prompt = no
distinguished_name = dn
x509_extensions = root_ext

[ dn ]
CN = ${root_name} CA

[ root_ext ]
basicConstraints = critical, CA:true
keyUsage = critical, keyCertSign, cRLSign
EOF
    openssl req -x509 -newkey rsa:2048 -nodes \
        -keyout "$dir/${root_name}.key" \
        -out "$dir/${root_name}.crt" \
        -days 1 -config "$dir/${root_name}.cnf" \
        -batch 2>/dev/null

    # Intermediate CA
    cat <<EOF > "$dir/${inter_name}.cnf"
[ req ]
prompt = no
distinguished_name = dn

[ dn ]
CN = ${inter_name} CA
EOF
    openssl req -newkey rsa:2048 -nodes \
        -keyout "$dir/${inter_name}.key" \
        -out "$dir/${inter_name}.csr" \
        -config "$dir/${inter_name}.cnf" \
        -batch 2>/dev/null
    cat <<EOF > "$dir/${inter_name}.ext"
basicConstraints = critical, CA:true
keyUsage = critical, digitalSignature, cRLSign, keyCertSign
EOF
    openssl x509 -req -in "$dir/${inter_name}.csr" \
        -CA "$dir/${root_name}.crt" -CAkey "$dir/${root_name}.key" \
        -CAcreateserial -out "$dir/${inter_name}.crt" -days 1 \
        -extfile "$dir/${inter_name}.ext" 2>/dev/null

    # Leaf cert
    cat <<EOF > "$dir/${leaf_name}.cnf"
[ req ]
prompt = no
distinguished_name = dn

[ dn ]
CN = ${leaf_name}
EOF
    openssl req -newkey rsa:2048 -nodes \
        -keyout "$dir/${leaf_name}.key" \
        -out "$dir/${leaf_name}.csr" \
        -config "$dir/${leaf_name}.cnf" \
        -batch 2>/dev/null
    cat <<EOF > "$dir/${leaf_name}.ext"
basicConstraints = CA:false
keyUsage = critical, digitalSignature
EOF
    openssl x509 -req -in "$dir/${leaf_name}.csr" \
        -CA "$dir/${inter_name}.crt" -CAkey "$dir/${inter_name}.key" \
        -CAcreateserial -out "$dir/${leaf_name}.crt" -days 1 \
        -extfile "$dir/${leaf_name}.ext" 2>/dev/null
}

extract_cms_signature() {
    local archive="$1"
    local der_file="$2"
    local data_file="$3"

    local comment
    if command -v zipnote >/dev/null 2>&1; then
        comment=$(zipnote "$archive" | awk '/^@ \(zip file comment below this line\)$/{getline; print}' | tr -d '\r')
    else
        comment=$(unzip -z "$archive" | sed -n '1!p' | tr -d '\r')
    fi

    local prefix="ZipSign=data:application/cms;base64,"
    [[ "$comment" == "$prefix"* ]] || return 1
    local b64_sig="${comment#"$prefix"}"

    printf '%s' "$b64_sig" | openssl base64 -d -A -out "$der_file"

    local file_size
    file_size=$(wc -c < "$archive" | tr -d '[:space:]')
    local signed_len=$(( file_size - ${#comment} - 2 ))
    head -c "$signed_len" "$archive" > "$data_file"
}

