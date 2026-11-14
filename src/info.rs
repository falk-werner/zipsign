use crate::error::Error;
use crate::ffi;
use crate::signer::ZIPSIGN_SIGNATURE_PREFIX;
use crate::zip::Zip;
use base64::prelude::*;
use foreign_types::ForeignType;
use openssl::cms::CmsContentInfo;
use openssl_sys as ossl_ffi;
use std::path::Path;

pub fn run_info<P: AsRef<Path>>(path: P) -> Result<(), Error> {
    let zip = Zip::new(path.as_ref());
    let comment = zip.get_comment()?;
    if !comment.starts_with(ZIPSIGN_SIGNATURE_PREFIX) {
        return Err(Error::Custom("missing signature".to_string()));
    }

    let b64 = &comment[ZIPSIGN_SIGNATURE_PREFIX.len()..];
    let der = BASE64_STANDARD
        .decode(b64)
        .map_err(|e| Error::Custom(e.to_string()))?;
    let cms = CmsContentInfo::from_der(&der).map_err(|e| Error::Cms(e.to_string()))?;

    unsafe {
        let bio = ossl_ffi::BIO_new(ossl_ffi::BIO_s_mem());
        if bio.is_null() {
            return Err(Error::Cms("failed to create BIO".to_string()));
        }
        ffi::CMS_ContentInfo_print_ctx(bio, cms.as_ptr(), 0, std::ptr::null());
        let mut ptr = std::ptr::null_mut();
        let len = ossl_ffi::BIO_get_mem_data(bio, &mut ptr);
        if len > 0 && !ptr.is_null() {
            let slice = std::slice::from_raw_parts(ptr as *const u8, len as usize);
            print!("{}", String::from_utf8_lossy(slice));
        }
        ffi::BIO_free(bio);
    }

    Ok(())
}
