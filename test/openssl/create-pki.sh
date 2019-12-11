#!/bin/bash
set -x

CONFIG_DIR="${1:-.}"

if [ ! -f "${CONFIG_DIR}/root-ca.conf" ] ; then
echo "error: missing ${CONFIG_DIR}/root-ca.conf"
exit 1
fi

which openssl
if [ "0" -ne "$?" ] ; then
echo "error: openssl needed"
exit 1
fi

rm -rf self-signed
rm -rf ca
rm -rf crl
rm -rf certs
rm -f keyring.pem

mkdir self-signed
openssl req -x509 \
    -newkey rsa:4096 \
    -keyout self-signed/key.pem \
    -out self-signed/cert.pem \
    -days 365 \
    -nodes \
    -batch

mkdir -p ca/root-ca/private
mkdir -p ca/root-ca/db
mkdir crl
mkdir certs

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

cp ca/root-ca.crt keyring.pem