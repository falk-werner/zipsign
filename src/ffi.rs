use openssl_sys as ffi;
use std::os::raw::{c_char, c_int, c_uint, c_void};

pub const CMS_NOCERTS: c_uint = 0x2;
pub const CMS_NO_CONTENT_VERIFY: c_uint = 0x4;
pub const CMS_NOINTERN: c_uint = 0x10;
pub const CMS_NO_SIGNER_CERT_VERIFY: c_uint = 0x20;
pub const CMS_DETACHED: c_uint = 0x40;
pub const CMS_BINARY: c_uint = 0x80;
pub const CMS_PARTIAL: c_uint = 0x4000;

extern "C" {
    pub fn CMS_sign(
        signcert: *mut ffi::X509,
        pkey: *mut ffi::EVP_PKEY,
        certs: *mut ffi::stack_st_X509,
        data: *mut ffi::BIO,
        flags: c_uint,
    ) -> *mut ffi::CMS_ContentInfo;

    pub fn CMS_add1_signer(
        cms: *mut ffi::CMS_ContentInfo,
        signcert: *mut ffi::X509,
        pkey: *mut ffi::EVP_PKEY,
        md: *const ffi::EVP_MD,
        flags: c_uint,
    ) -> *mut c_void;

    pub fn CMS_final(
        cms: *mut ffi::CMS_ContentInfo,
        data: *mut ffi::BIO,
        dcont: *mut ffi::BIO,
        flags: c_uint,
    ) -> c_int;

    pub fn CMS_verify(
        cms: *mut ffi::CMS_ContentInfo,
        certs: *mut ffi::stack_st_X509,
        store: *mut ffi::X509_STORE,
        dcont: *mut ffi::BIO,
        out: *mut ffi::BIO,
        flags: c_uint,
    ) -> c_int;

    pub fn CMS_get1_certs(cms: *mut ffi::CMS_ContentInfo) -> *mut ffi::stack_st_X509;

    pub fn CMS_ContentInfo_print_ctx(
        out: *mut ffi::BIO,
        cms: *mut ffi::CMS_ContentInfo,
        indent: c_int,
        pctx: *const c_void,
    ) -> c_int;

    pub fn BIO_new_mem_buf(buf: *const c_void, len: c_int) -> *mut ffi::BIO;
    pub fn CMS_ContentInfo_free(cms: *mut ffi::CMS_ContentInfo);

    pub fn BIO_free(a: *mut ffi::BIO) -> c_int;

    pub fn X509_STORE_load_locations(
        store: *mut ffi::X509_STORE,
        file: *const c_char,
        dir: *const c_char,
    ) -> c_int;

    pub fn X509_STORE_CTX_new() -> *mut ffi::X509_STORE_CTX;
    pub fn X509_STORE_CTX_free(ctx: *mut ffi::X509_STORE_CTX);
    pub fn X509_STORE_CTX_init(
        ctx: *mut ffi::X509_STORE_CTX,
        store: *mut ffi::X509_STORE,
        x509: *mut ffi::X509,
        chain: *mut ffi::stack_st_X509,
    ) -> c_int;
    pub fn X509_STORE_CTX_set0_untrusted(
        ctx: *mut ffi::X509_STORE_CTX,
        sk: *mut ffi::stack_st_X509,
    );
    pub fn X509_verify_cert(ctx: *mut ffi::X509_STORE_CTX) -> c_int;
}
