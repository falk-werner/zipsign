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

## Known Limitations

-   ZIP comments are limited to a maximum of 64 KBytes.  
    CMS allows to include signer certificates, intermediate certificates as well as multiple signatures.
    When overused, this might not fit into the comment.

-   Implementation limitations
    - *sign:* including signer certificate is not supported yet
    - *sign:* including intermediate certificates is not supported yet
    - *sign:* multiple signatures are not supported yet
    - *verify:* providing PKI for verification is not supported yet

## External references

-   [ZIP file format](https://en.wikipedia.org/wiki/Zip_(file_format))
-   [CMS](https://en.wikipedia.org/wiki/Cryptographic_Message_Syntax)

