use std::fmt;

#[derive(Debug)]
pub enum Error {
    FileNotFound(String),
    ParseCertificate(String),
    ParsePrivateKey(String),
    #[allow(dead_code)]
    KeyMismatch,
    Zip(String),
    Cms(String),
    Custom(String),
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Error::FileNotFound(path) => write!(f, "file not found: {}", path),
            Error::ParseCertificate(msg) => write!(f, "failed to parse certificate: {}", msg),
            Error::ParsePrivateKey(msg) => write!(f, "failed to parse key file: {}", msg),
            Error::KeyMismatch => write!(f, "private key does not match certificate"),
            Error::Zip(msg) => write!(f, "{}", msg),
            Error::Cms(msg) => write!(f, "{}", msg),
            Error::Custom(msg) => write!(f, "{}", msg),
        }
    }
}

impl std::error::Error for Error {}
