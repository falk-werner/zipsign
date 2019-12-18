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

    zipsign sign -f <filename> -p <private-key> -c <signers certificate> [-i <intemerdiate> ...]

| Short | Long           | Argument    | Description                                              |
| ----- | -------------- | ----------- | -------------------------------------------------------- |
| -f    | --file         | archive.zip | Required. ZIP file to sign.                              |
| -p    | --private-key  | key.pem     | Required. Filename of signers private key.               |
| -c    | --certificate  | cert.pem    | Required. Filename of signers certificate.               |
| -i    | --intermediate | inter.pem   | Optional. Filename of intermediate certifiacte to embed. |
| -v    | --verbose      | _none_      | Optional. Enable additional output.                      |

During signing the ZIP file will be altered (ZIP comment will be re-written).

It is possible to specifify none, one or more intermediate certificates, that will be embedded in
the signature. Embedding intermediate certificates are used to close then chain of trust for later
verfication.

### Verify

    zipsign verify -f <filename> -c <signers certificate> [-k <keyring>]

| Short | Long          | Argument    | Description                                |
| ----- | ------------- | ----------- | ------------------------------------------ |
| -f    | --file        | archive.zip | Required. ZIP file to verify.              |
| -c    | --certificate | cert.pem    | Required. Filename of signers certificate. |
| -k    | --keyring     | keyring.pem | Optional. Filename of PKI.                 |
| -v    | --verbose     | _none_      | Optional. Enable additional output.        |

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

## Known Limitations

-   ZIP comments are limited to a maximum of 64 KBytes.  
    CMS allows to include signer certificates, intermediate certificates as well as multiple signatures.
    When overused, this might not fit into the comment.

-   Implementation limitations
    - *sign:* including signer certificate is not supported yet
    - *sign:* multiple signatures are not supported yet

# Build

To build the project, cmake is used.

    mkdir build
    cd build
    cmake ..
    make
    sudo make install

## Dependencies

-   [OpenSSL](https://www.openssl.org/) (libssl-dev)
-   [Google Test](https://github.com/google/googletest) *(Test only)*

## External references

-   [ZIP file format](https://en.wikipedia.org/wiki/Zip_(file_format))
-   [CMS](https://en.wikipedia.org/wiki/Cryptographic_Message_Syntax)
-   [PKI tutorial](https://pki-tutorial.readthedocs.io/en/latest/)