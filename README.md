[![Build Status](https://travis-ci.org/falk-werner/zipsign.svg?branch=master)](https://travis-ci.org/falk-werner/zipsign)
[![codecov](https://codecov.io/gh/falk-werner/zipsign/branch/master/graph/badge.svg)](https://codecov.io/gh/falk-werner/zipsign)

# zipsign
Sign and verify ZIP archives.

## tl;dr

-   create private key and certificate for signing  
    `openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes`

-   create a zip archive  
    `echo 42 > answer.txt`  
    `zip -r archive.zip answer.txt`

-   sign the archive  
    `zipsign sign -f archive.zip -p key.pem -c cert.pem`

-   verify signature of the archive  
    `zipsign verify -f archive.zip -c cert.pem`

## Command line options

### Sign

Sign an archive.

    zipsign sign -f <filename> -p <private-key> -c <signers certificate> [-i <intemerdiate> ...]

| Short | Long                | Argument    | Description                                              |
| ----- | ------------------- | ----------- | -------------------------------------------------------- |
| -f    | --file              | archive.zip | Required. ZIP file to sign.                              |
| -p    | --private-key       | key.pem     | Required. Filename of signers private key.               |
| -c    | --certificate       | cert.pem    | Required. Filename of signers certificate.               |
| -i    | --intermediate      | inter.pem   | Optional. Filename of intermediate certifiacte to embed. |
| -e    | --embed-certificate | _none_      | Optional. Embed signers certificate in signature.        |
| -v    | --verbose           | _none_      | Optional. Enable additional output.                      |

During signing the ZIP file will be altered (ZIP comment will be re-written).

It is possible to specifify none, one or more intermediate certificates, that will be embedded in
the signature. Embedding intermediate certificates are used to close then chain of trust for later
verfication.

It is also possible to specify one or more signers by adding private key and certificate pairs.  
_Note_: It is recommended to embed signers certificates when using multiple signers.

### Verify

Verfiy an archive.

    zipsign verify -f <filename> -c <signers certificate> [-k <keyring>]

| Short | Long          | Argument    | Description                                |
| ----- | ------------- | ----------- | ------------------------------------------ |
| -f    | --file        | archive.zip | Required. ZIP file to verify.              |
| -c    | --certificate | cert.pem    | Required. Filename of signers certificate. |
| -k    | --keyring     | keyring.pem | Optional. Filename of PKI.                 |
| -v    | --verbose     | _none_      | Optional. Enable additional output.        |

If the archive is signed by multiple signers, multiple certificates can be
specified for validation.

_Note_: Verfication is done only against the given certificates. Other signers are
ignored, even if the archive is signed by multiple signers.

_Note_: Only one keyring can be specified.

### Info

Prints signature of ZIP archive in human readable form.

    zipsign info -f <filename>

| Short | Long          | Argument    | Description                                |
| ----- | ------------- | ----------- | ------------------------------------------ |
| -f    | --file        | archive.zip | Required. ZIP file to print info.          |


## How it works

[ZIP file format](https://en.wikipedia.org/wiki/Zip_(file_format)) specifies a comment field, 
which is located at the very end of the archive.
This allows to create a [CMS](https://en.wikipedia.org/wiki/Cryptographic_Message_Syntax) signature
over the full archive (excluding the comment) and stores it Base64-encoded as new comment.

For verification, the comment is read from the archive and verified against archives contents.

## Signature format

ZipSign used the following format for the signature

    ZipSign=data:application/cms;base64,<signature-data>

The signature data consists of base64 encoded PEM data.

To print the signature from a previously signed ZIP archive, one can use the following command.

    > unzip -z archive.zip
    Archive:   archive.zip
    ZipSign=data:application/cms;base64,MIIDow...

## Sign and verify using keyring (PKI)

Verification using a PKI is supported via `--keyring` option during validation.  
Setup a CA suitable for signing is a complex topic. A good starting point might
be the PKI tutorial at
[https://pki-tutorial.readthedocs.io/en/latest/](https://pki-tutorial.readthedocs.io/en/latest/).

The test directory contains a bash script [create-pki.sh](test/openssl/create-pki.sh), which
creates a small PKI used within unit tests.

    # sign archive alice key and certificate
    zipsign sign -f archive.zip -p certs/alice.key -c certs/alice.crt

    # verify archive using keyring
    zipsign verify -f archive.zip -c certs/alice.crt -k keyring.pem

## Include intermediate certificates

To complete chain of trust, on or more intermediate certificates can be specified.

    # sign archive with alice's key and certificate, add signing CA as intermediate
    zipsign sign -f archive.zip -p certs/alice.key -c certs/alice.crt -i ca/signing-ca.crt

    # verify archive using root CA as keyring
    zipsign verify -f archive.zip -c certs/alice.crt -k ca/root-ca.crt

## Multiple signers

An archive can be signed by multiple signers.  
This might come in handy during certiface changes, e.g. when one certificate will expire
shortly and the new one should be used simultaniously.

To sign an archive with multiple signers, you can specify one certificate and one private
key per signer. The certificates should be embedded in the archives signature, so they
can be located during verification.

    zipsign sign -f archive.zip \
      -p certs/alice.key -c certs/alice.cert \
      -p self-signed/key.pem -c self-signed/cert.pem \
      -e

During verification, you can choose the proper signer (or both).

    # verify against self signed certificate
    zipsign verfiy -f archive.zip -c self-signed/cert.pem

    # verify against alice's certificates
    zipsign verify -f archive.zip -c certs/alice.crt -k keyring.pem

    # verify against both certificates
    zipsign verify -f archive.zip \
      -c self-signed/cert.pem \
      -c certs/alice.crt -k keyring.pem

## Known Limitations

-   ZIP comments are limited to a maximum of 64 KBytes.  
    CMS allows to include signer certificates, intermediate certificates as well as multiple signatures.
    When overused, this might not fit into the comment.

# Build

To build the project, cmake is used.

    mkdir build
    cd build
    cmake ..
    make
    sudo make install

## Build using Docker (CentOS 7)

    docker build --rm --build-arg "USER_ID=`id -u`" --tag zipsign-centos -f docker/centos7.dockerfile .

To run the docker container, use

    docker run --rm --user="`id -u`" -it zipsign-centos

## Dependencies

-   [OpenSSL>=1.1](https://www.openssl.org/) (libssl-dev)
-   [Google Test](https://github.com/google/googletest) *(Test only)*

## External references

-   [ZIP file format](https://en.wikipedia.org/wiki/Zip_(file_format))
-   [CMS](https://en.wikipedia.org/wiki/Cryptographic_Message_Syntax)
-   [PKI tutorial](https://pki-tutorial.readthedocs.io/en/latest/)