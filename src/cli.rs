pub const VERSION: &str = env!("CARGO_PKG_VERSION");

pub fn print_general_usage() {
    println!(
        r#"zipsign v{VERSION}, Copyright (c) 2019-2026 Falk Werner
Signs and verifies ZIP archives

Usage:
    zipsign sign <args...> | verify <args...> | info <args...> | -h

Verbs:
    sign                    Signs a zip archive.
    verify                  Verifies the signature of a zip archive.
    info                    Print info about the signature of zip archive.
    -V, --version           Print version.
    -h, --help              Print usage.

Examples:
    Sign:
        zipsign sign -f archive.zip -p key.pem -c cert.pem
    Verify:
        zipsign verify -f archive.zip -c cert.pem
        zipsign verify -f archive.zip -c cert.pem --self-signed"#
    );
}

pub fn print_sign_usage() {
    println!(
        r#"zipsign v{VERSION}, Copyright (c) 2019-2026 Falk Werner
Signs and verifies ZIP archives

sign: Signs a zip archive.

Usage:
    zipsign sign -f <value> -p <value> -c <value> [-i <value>] [-e] [-v] | -h

Arguments:
    -f, --file                      Required. Archive to sign.
    -p, --private-key               Required. Private key to sign. Can be used multiple times.
    -c, --certificate               Required. Certificate of signer. Can be used multiple times.
    -i, --intermediate              Add intermediate certificate.  Can be used multiple times.
    -e, --embed-certificate         Embed signers certificate in signature.
    -v, --verbose                   Enable additional output.
    -h, --help                      Print usage."#
    );
}

pub fn print_verify_usage() {
    println!(
        r#"zipsign v{VERSION}, Copyright (c) 2019-2026 Falk Werner
Signs and verifies ZIP archives

verify: Verifies the signature of a zip archive.

Usage:
    zipsign verify -f <value> -c <value> [-k <value>] [-v] [-s] | -h

Arguments:
    -f, --file                      Required. Archive to verify.
    -c, --certificate               Required. Certificate of signer. Can be used multiple times.
    -k, --keyring                   Path of keyring file.
    -v, --verbose                   Enable additionl output.
    -s, --self-signed               Allows self signed certificates, skip cert verify.
    -h, --help                      Print usage."#
    );
}

pub fn print_info_usage() {
    println!(
        r#"zipsign v{VERSION}, Copyright (c) 2019-2026 Falk Werner
Signs and verifies ZIP archives

info: Print info about the signature of zip archive.

Usage:
    zipsign info -f <value> | -h

Arguments:
    -f, --file                      Required. Archive to verify.
    -h, --help                      Print usage."#
    );
}


pub struct SignArgs {
    pub file: Option<String>,
    pub keys: Vec<String>,
    pub certs: Vec<String>,
    pub intermediates: Vec<String>,
    pub embed_cert: bool,
    pub verbose: bool,
    pub help: bool,
    pub unrecognized: bool,
}

pub fn parse_sign_args(args: &[String]) -> SignArgs {
    let mut file = None;
    let mut keys = Vec::new();
    let mut certs = Vec::new();
    let mut intermediates = Vec::new();
    let mut embed_cert = false;
    let mut verbose = false;
    let mut help = false;
    let mut unrecognized = false;

    let mut i = 0;
    while i < args.len() {
        let arg = &args[i];
        if arg == "-h" || arg == "--help" {
            help = true;
            break;
        } else if arg == "-e" || arg == "--embed-certificate" {
            embed_cert = true;
            i += 1;
        } else if arg == "-v" || arg == "--verbose" {
            verbose = true;
            i += 1;
        } else if arg == "-f" || arg == "--file" {
            i += 1;
            if i < args.len() {
                file = Some(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--file=") {
            file = Some(val.to_string());
            i += 1;
        } else if arg.starts_with("-f") && arg.len() > 2 {
            file = Some(arg[2..].to_string());
            i += 1;
        } else if arg == "-p" || arg == "--private-key" {
            i += 1;
            if i < args.len() {
                keys.push(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--private-key=") {
            keys.push(val.to_string());
            i += 1;
        } else if arg.starts_with("-p") && arg.len() > 2 {
            keys.push(arg[2..].to_string());
            i += 1;
        } else if arg == "-c" || arg == "--certificate" {
            i += 1;
            if i < args.len() {
                certs.push(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--certificate=") {
            certs.push(val.to_string());
            i += 1;
        } else if arg.starts_with("-c") && arg.len() > 2 {
            certs.push(arg[2..].to_string());
            i += 1;
        } else if arg == "-i" || arg == "--intermediate" {
            i += 1;
            if i < args.len() {
                intermediates.push(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--intermediate=") {
            intermediates.push(val.to_string());
            i += 1;
        } else if arg.starts_with("-i") && arg.len() > 2 {
            intermediates.push(arg[2..].to_string());
            i += 1;
        } else {
            unrecognized = true;
            break;
        }
    }

    SignArgs {
        file,
        keys,
        certs,
        intermediates,
        embed_cert,
        verbose,
        help,
        unrecognized,
    }
}

pub struct VerifyArgs {
    pub file: Option<String>,
    pub certs: Vec<String>,
    pub keyring: Option<String>,
    pub self_signed: bool,
    pub verbose: bool,
    pub help: bool,
    pub unrecognized: bool,
}

pub fn parse_verify_args(args: &[String]) -> VerifyArgs {
    let mut file = None;
    let mut certs = Vec::new();
    let mut keyring = None;
    let mut self_signed = false;
    let mut verbose = false;
    let mut help = false;
    let mut unrecognized = false;

    let mut i = 0;
    while i < args.len() {
        let arg = &args[i];
        if arg == "-h" || arg == "--help" {
            help = true;
            break;
        } else if arg == "-s" || arg == "--self-signed" {
            self_signed = true;
            i += 1;
        } else if arg == "-v" || arg == "--verbose" {
            verbose = true;
            i += 1;
        } else if arg == "-f" || arg == "--file" {
            i += 1;
            if i < args.len() {
                file = Some(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--file=") {
            file = Some(val.to_string());
            i += 1;
        } else if arg.starts_with("-f") && arg.len() > 2 {
            file = Some(arg[2..].to_string());
            i += 1;
        } else if arg == "-c" || arg == "--certificate" {
            i += 1;
            if i < args.len() {
                certs.push(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--certificate=") {
            certs.push(val.to_string());
            i += 1;
        } else if arg.starts_with("-c") && arg.len() > 2 {
            certs.push(arg[2..].to_string());
            i += 1;
        } else if arg == "-k" || arg == "--keyring" {
            i += 1;
            if i < args.len() {
                keyring = Some(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--keyring=") {
            keyring = Some(val.to_string());
            i += 1;
        } else if arg.starts_with("-k") && arg.len() > 2 {
            keyring = Some(arg[2..].to_string());
            i += 1;
        } else {
            unrecognized = true;
            break;
        }
    }

    VerifyArgs {
        file,
        certs,
        keyring,
        self_signed,
        verbose,
        help,
        unrecognized,
    }
}

pub struct InfoArgs {
    pub file: Option<String>,
    pub help: bool,
    pub unrecognized: bool,
}

pub fn parse_info_args(args: &[String]) -> InfoArgs {
    let mut file = None;
    let mut help = false;
    let mut unrecognized = false;

    let mut i = 0;
    while i < args.len() {
        let arg = &args[i];
        if arg == "-h" || arg == "--help" {
            help = true;
            break;
        } else if arg == "-f" || arg == "--file" {
            i += 1;
            if i < args.len() {
                file = Some(args[i].clone());
                i += 1;
            } else {
                unrecognized = true;
                break;
            }
        } else if let Some(val) = arg.strip_prefix("--file=") {
            file = Some(val.to_string());
            i += 1;
        } else if arg.starts_with("-f") && arg.len() > 2 {
            file = Some(arg[2..].to_string());
            i += 1;
        } else {
            unrecognized = true;
            break;
        }
    }

    InfoArgs {
        file,
        help,
        unrecognized,
    }
}

