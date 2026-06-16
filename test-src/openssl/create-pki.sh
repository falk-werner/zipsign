#!/bin/bash
set -x
set -e

CONFIG_DIR="${1:-.}"

# check prerequisites

if [ ! -f "${CONFIG_DIR}/root-ca.conf" ] ; then
echo "error: missing ${CONFIG_DIR}/root-ca.conf"
exit 1
fi

if [ ! -f "${CONFIG_DIR}/signing-ca.conf" ] ; then
echo "error: missing ${CONFIG_DIR}/signing-ca.conf"
exit 1
fi

if [ ! -f "${CONFIG_DIR}/alice.conf" ] ; then
echo "error: missing ${CONFIG_DIR}/alice.conf"
exit 1
fi

which openssl
if [ "0" -ne "$?" ] ; then
echo "error: openssl needed"
exit 1
fi

# clean up (previously generated files)

rm -rf self-signed
rm -rf ca
rm -rf crl
rm -rf certs
rm -f keyring.pem

# create self signed certificate (and key)

mkdir self-signed
openssl req -x509 \
    -newkey rsa:4096 \
    -keyout self-signed/key.pem \
    -out self-signed/cert.pem \
    -days 365 \
    -nodes \
    -batch

# create Root CA

mkdir -p ca/root-ca/private
mkdir -p ca/root-ca/db
mkdir -p crl
mkdir -p certs

chmod 700 ca/root-ca/private

cp /dev/null ca/root-ca/db/root-ca.db
cp /dev/null ca/root-ca/db/root-ca.db.attr
echo 01 > ca/root-ca/db/root-ca.crt.srl
echo 01 > ca/root-ca/db/root-ca.crl.srl

openssl req -new \
    -nodes \
    -config ${CONFIG_DIR}/root-ca.conf \
    -out ca/root-ca.csr \
    -keyout ca/root-ca/private/root-ca.key

openssl ca -selfsign \
    -config ${CONFIG_DIR}/root-ca.conf \
    -in ca/root-ca.csr \
    -out ca/root-ca.crt \
    -extensions root_ca_ext \
    -batch

# create signing CA

mkdir -p ca/signing-ca/private
mkdir -p ca/signing-ca/db
mkdir -p crl
mkdir -p certs

chmod 700 ca/signing-ca/private

cp /dev/null ca/signing-ca/db/signing-ca.db
cp /dev/null ca/signing-ca/db/signing-ca.db.attr
echo 01 > ca/signing-ca/db/signing-ca.crt.srl
echo 01 > ca/signing-ca/db/signing-ca.crl.srl

openssl req -new \
    -nodes \
    -config ${CONFIG_DIR}/signing-ca.conf \
    -out ca/signing-ca.csr \
    -keyout ca/signing-ca/private/signing-ca.key \
    -batch

openssl ca \
    -config ${CONFIG_DIR}/root-ca.conf \
    -in ca/signing-ca.csr \
    -out ca/signing-ca.crt \
    -extensions signing_ca_ext \
    -batch

# create certificate and key for Alice

openssl req -new \
    -nodes \
    -config ${CONFIG_DIR}/alice.conf \
    -out certs/alice.csr \
    -keyout certs/alice.key \
    -batch

openssl ca \
    -config ${CONFIG_DIR}/signing-ca.conf \
    -in certs/alice.csr \
    -out certs/alice.crt \
    -extensions email_ext \
    -batch

# create key ring

cat ca/root-ca.crt \
    ca/signing-ca.crt \
    > keyring.pem