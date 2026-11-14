use crate::error::Error;
use crate::ffi;
use crate::zip::Zip;
use base64::prelude::*;
use foreign_types::ForeignType;
use openssl::cms::CmsContentInfo;
use openssl::pkey::{PKey, Private};
use openssl::stack::Stack;
use openssl::x509::X509;
use std::os::raw::{c_int, c_void};
use std::path::Path;

pub const ZIPSIGN_SIGNATURE_PREFIX: &str = "ZipSign=data:application/cms;base64,";

pub struct Signer {
    keys: Vec<PKey<Private>>,
    certs: Vec<X509>,
    intermediates: Vec<X509>,
    embed_certs: bool,
    #[allow(dead_code)]
    verbose: bool,
}

impl Signer {
    pub fn new(embed_certs: bool, verbose: bool) -> Self {
        Self {
            keys: Vec::new(),
            certs: Vec::new(),
            intermediates: Vec::new(),
            embed_certs,
            verbose,
        }
    }

    pub fn add_signer<P: AsRef<Path>>(&mut self, key_path: P, cert_path: P) -> Result<(), Error> {
        let key = Self::load_key(key_path.as_ref())?;
        let cert = Self::load_cert(cert_path.as_ref())?;
        self.keys.push(key);
        self.certs.push(cert);
        Ok(())
    }

    pub fn add_intermediate<P: AsRef<Path>>(&mut self, cert_path: P) -> Result<(), Error> {
        let cert = Self::load_cert(cert_path.as_ref())?;
        self.intermediates.push(cert);
        Ok(())
    }

    fn load_cert(path: &Path) -> Result<X509, Error> {
        let content = std::fs::read(path).map_err(|e| {
            if e.kind() == std::io::ErrorKind::NotFound {
                Error::FileNotFound(path.display().to_string())
            } else {
                Error::Custom(format!("failed to open file {}: {}", path.display(), e))
            }
        })?;
        X509::from_pem(&content).map_err(|e| {
            Error::ParseCertificate(format!("failed to parse certificate file (OpenSSL: {})", e))
        })
    }

    fn load_key(path: &Path) -> Result<PKey<Private>, Error> {
        let content = std::fs::read(path).map_err(|e| {
            if e.kind() == std::io::ErrorKind::NotFound {
                Error::FileNotFound(path.display().to_string())
            } else {
                Error::Custom(format!("failed to open file {}: {}", path.display(), e))
            }
        })?;
        PKey::private_key_from_pem(&content).map_err(|e| {
            Error::ParsePrivateKey(format!("failed to parse key file (OpenSSL: {})", e))
        })
    }

    pub fn sign<P: AsRef<Path>>(&self, zip_path: P) -> Result<(), Error> {
        let zip = Zip::new(zip_path.as_ref());
        let data = zip.read_data_to_sign()?;

        let mut inter_stack = Stack::new().map_err(|e| Error::Cms(e.to_string()))?;
        for inter in &self.intermediates {
            inter_stack
                .push(inter.clone())
                .map_err(|e| Error::Cms(e.to_string()))?;
        }

        let mut flags = ffi::CMS_DETACHED | ffi::CMS_BINARY;
        if !self.embed_certs {
            flags |= ffi::CMS_NOCERTS;
        }

        unsafe {
            let bio = ffi::BIO_new_mem_buf(data.as_ptr() as *const c_void, data.len() as c_int);
            if bio.is_null() {
                return Err(Error::Cms("failed to create BIO".to_string()));
            }

            let inter_stack_ptr = if self.intermediates.is_empty() {
                std::ptr::null_mut()
            } else {
                inter_stack.as_ptr()
            };

            let cms_ptr = ffi::CMS_sign(
                std::ptr::null_mut(),
                std::ptr::null_mut(),
                inter_stack_ptr,
                bio,
                flags | ffi::CMS_PARTIAL,
            );
            if cms_ptr.is_null() {
                ffi::BIO_free(bio);
                let err = openssl::error::ErrorStack::get();
                return Err(Error::Cms(format!("unable to sign file: {}", err)));
            }

            for (cert, key) in self.certs.iter().zip(self.keys.iter()) {
                let signer = ffi::CMS_add1_signer(
                    cms_ptr,
                    cert.as_ptr(),
                    key.as_ptr(),
                    std::ptr::null(),
                    flags | ffi::CMS_PARTIAL,
                );
                if signer.is_null() {
                    ffi::CMS_ContentInfo_free(cms_ptr);
                    ffi::BIO_free(bio);
                    let err = openssl::error::ErrorStack::get();
                    return Err(Error::Cms(format!("failed to add signer: {}", err)));
                }
            }

            let rc = ffi::CMS_final(cms_ptr, bio, std::ptr::null_mut(), flags);
            ffi::BIO_free(bio);
            if rc != 1 {
                ffi::CMS_ContentInfo_free(cms_ptr);
                let err = openssl::error::ErrorStack::get();
                return Err(Error::Cms(format!("finalize CMS failed: {}", err)));
            }

            let cms = CmsContentInfo::from_ptr(cms_ptr);
            let der = cms.to_der().map_err(|e| Error::Cms(e.to_string()))?;
            let b64 = BASE64_STANDARD.encode(&der);
            let comment = format!("{}{}", ZIPSIGN_SIGNATURE_PREFIX, b64);
            zip.set_comment(&comment)?;
        }

        Ok(())
    }
}
