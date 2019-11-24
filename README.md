[![Build Status](https://travis-ci.org/falk-werner/zipsign.svg?branch=master)](https://travis-ci.org/falk-werner/zipsign)

# zipsign
Sign and verify ZIP archives

## Signature

Signature is stored within ZIP comment as base64 encoded CMS signature.

ZipSign=data:application/cms;base64,<cms-data>;

## Writing and reading ZIP comments

### Write ZIP comment

    echo "my comment" | zip -z archive.zip

### Reading ZIP comment

    unzip -z archive.zip

