use crate::error::Error;
use crate::ffi;
use crate::signer::ZIPSIGN_SIGNATURE_PREFIX;
use crate::zip::Zip;
use base64::prelude::*;
use foreign_types::ForeignType;
use openssl::cms::CmsContentInfo;
use openssl::stack::Stack;
use openssl::x509::store::X509StoreBuilder;
use openssl::x509::X509;
use openssl_sys as ossl_ffi;
use std::ffi::CString;
use std::fmt;
use std::os::raw::{c_int, c_void};
use std::path::Path;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum VerifyResult {
    Good = 0,
    Bad = 1,
    BadMissingSignature = 2,
    BadInvalidCertificateChain = 3,
    BadInvalidSignature = 4,
}

impl fmt::Display for VerifyResult {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            VerifyResult::Good => write!(f, "OK"),
            VerifyResult::BadMissingSignature => write!(f, "INVALID_MISSING_SIGNATURE"),
            VerifyResult::BadInvalidCertificateChain => write!(f, "INVALID_CERTIFICATE_CHAIN"),
            VerifyResult::BadInvalidSignature => write!(f, "INVALID_SIGNATURE"),
            VerifyResult::Bad => write!(f, "INVALID"),
        }
    }
}

pub struct Verifier {
    signers: Vec<X509>,
}

impl Verifier {
    pub fn new() -> Self {
        Self { signers: Vec::new() }
    }

    pub fn add_certificate<P: AsRef<Path>>(&mut self, path: P) -> Result<(), Error> {
        let content = std::fs::read(path.as_ref()).map_err(|e| {
            if e.kind() == std::io::ErrorKind::NotFound {
                Error::FileNotFound(path.as_ref().display().to_string())
            } else {
                Error::Custom(format!("failed to open file {}: {}", path.as_ref().display(), e))
            }
        })?;
        let cert = X509::from_pem(&content).map_err(|e| {
            Error::ParseCertificate(format!("failed to parse certificate file (OpenSSL: {})", e))
        })?;
        self.signers.push(cert);
        Ok(())
    }

    pub fn verify<P: AsRef<Path>>(
        &self,
        filename: P,
        keyring_path: Option<&str>,
        verbose: bool,
        self_signed: bool,
    ) -> Result<VerifyResult, Error> {
        let zip = Zip::new(filename.as_ref());
        let comment = match zip.get_comment() {
            Ok(c) => c,
            Err(_) => {
                return Ok(VerifyResult::Bad);
            }
        };

        if !comment.starts_with(ZIPSIGN_SIGNATURE_PREFIX) {
            if verbose {
                eprintln!("error: missing signature");
            }
            return Ok(VerifyResult::BadMissingSignature);
        }

        let mut store_builder = X509StoreBuilder::new().map_err(|e| Error::Cms(e.to_string()))?;
        if let Some(keyring) = keyring_path {
            if !keyring.is_empty() {
                let c_path = CString::new(keyring).map_err(|e| Error::Custom(e.to_string()))?;
                unsafe {
                    let rc = ffi::X509_STORE_load_locations(
                        store_builder.as_ptr(),
                        c_path.as_ptr(),
                        std::ptr::null(),
                    );
                    if rc != 1 {
                        if verbose {
                            eprintln!("error: keyring file not found");
                        }
                        return Ok(VerifyResult::Bad);
                    }
                }
            }
        }

        let b64_sig = &comment[ZIPSIGN_SIGNATURE_PREFIX.len()..];
        let der = match BASE64_STANDARD.decode(b64_sig) {
            Ok(d) => d,
            Err(_) => {
                if verbose {
                    eprintln!("error: corrupted base64");
                }
                return Ok(VerifyResult::Bad);
            }
        };

        let cms = match CmsContentInfo::from_der(&der) {
            Ok(c) => c,
            Err(_) => {
                if verbose {
                    eprintln!("error: failed to parse CMS");
                }
                return Ok(VerifyResult::Bad);
            }
        };


        let mut cert_stack = Stack::new().map_err(|e| Error::Cms(e.to_string()))?;
        for cert in &self.signers {
            store_builder.add_cert(cert.clone()).map_err(|e| Error::Cms(e.to_string()))?;
            cert_stack.push(cert.clone()).map_err(|e| Error::Cms(e.to_string()))?;
        }
        let store = store_builder.build();

        unsafe {
            for cert in &self.signers {
                let untrusted = ffi::CMS_get1_certs(cms.as_ptr());
                if !self_signed {
                    let ctx = ffi::X509_STORE_CTX_new();
                    if ctx.is_null() {
                        Self::free_sk(untrusted);
                        return Ok(VerifyResult::BadInvalidCertificateChain);
                    }
                    let rc = ffi::X509_STORE_CTX_init(
                        ctx,
                        store.as_ptr(),
                        cert.as_ptr(),
                        std::ptr::null_mut(),
                    );
                    if rc != 1 {
                        ffi::X509_STORE_CTX_free(ctx);
                        Self::free_sk(untrusted);
                        return Ok(VerifyResult::BadInvalidCertificateChain);
                    }
                    if !untrusted.is_null() {
                        ffi::X509_STORE_CTX_set0_untrusted(ctx, untrusted);
                    }
                    let verify_rc = ffi::X509_verify_cert(ctx);
                    ffi::X509_STORE_CTX_free(ctx);
                    Self::free_sk(untrusted);
                    if verify_rc != 1 {
                        return Ok(VerifyResult::BadInvalidCertificateChain);
                    }
                } else {
                    Self::free_sk(untrusted);
                }
            }

            let data = zip.read_data_to_sign()?;

            let bio1 = ffi::BIO_new_mem_buf(data.as_ptr() as *const c_void, data.len() as c_int);
            let chain_flags = ffi::CMS_DETACHED
                | ffi::CMS_BINARY
                | ffi::CMS_NO_SIGNER_CERT_VERIFY
                | ffi::CMS_NO_CONTENT_VERIFY;
            let chain_valid = ffi::CMS_verify(
                cms.as_ptr(),
                cert_stack.as_ptr(),
                store.as_ptr(),
                bio1,
                std::ptr::null_mut(),
                chain_flags,
            );
            ffi::BIO_free(bio1);
            if chain_valid != 1 {
                return Ok(VerifyResult::BadInvalidCertificateChain);
            }

            let bio2 = ffi::BIO_new_mem_buf(data.as_ptr() as *const c_void, data.len() as c_int);
            let sig_flags = ffi::CMS_DETACHED
                | ffi::CMS_BINARY
                | ffi::CMS_NOINTERN
                | ffi::CMS_NO_SIGNER_CERT_VERIFY;
            let sig_valid = ffi::CMS_verify(
                cms.as_ptr(),
                cert_stack.as_ptr(),
                store.as_ptr(),
                bio2,
                std::ptr::null_mut(),
                sig_flags,
            );
            ffi::BIO_free(bio2);
            if sig_valid != 1 {
                return Ok(VerifyResult::BadInvalidSignature);
            }
        }

        Ok(VerifyResult::Good)
    }

    unsafe fn free_sk(sk: *mut ossl_ffi::stack_st_X509) {
        unsafe extern "C" fn free_x509(ptr: *mut std::ffi::c_void) {
            ossl_ffi::X509_free(ptr as *mut ossl_ffi::X509);
        }

        if !sk.is_null() {
            ossl_ffi::OPENSSL_sk_pop_free(
                sk as *mut ossl_ffi::OPENSSL_STACK,
                Some(free_x509),
            );
        }
    }

}
