pub const VERSION: &str = "1.7.0";

pub fn print_general_usage() {
    println!(
        "zipsign v{}, Copyright (c) 2019-2026 Falk Werner\n\
Signs and verifies ZIP archives\n\
\n\
Usage:\n\
\tzipsign sign <args...> | verify <args...> | info <args...> | -h\n\
\n\
Verbs:\n\
\tsign\t\t\tSigns a zip archive.\n\
\tverify\t\t\tVerifies the signature of a zip archive.\n\
\tinfo\t\t\tPrint info about the signature of zip archive.\n\
\t-V, --version\t\tPrint version.\n\
\t-h, --help\t\tPrint usage.\n\
\n\
Examples:\n\
\tSign:\n\
\t\tzipsign sign -f archive.zip -p key.pem -c cert.pem\n\
\tVerify:\n\
\t\tzipsign verify -f archive.zip -c cert.pem\n\
\t\tzipsign verify -f archive.zip -c cert.pem --self-signed",
        VERSION
    );
}

pub fn print_sign_usage() {
    println!(
        "zipsign v{}, Copyright (c) 2019-2026 Falk Werner\n\
Signs and verifies ZIP archives\n\
\n\
sign: Signs a zip archive.\n\
\n\
Usage:\n\
\tzipsign sign -f <value> -p <value> -c <value> [-i <value>] [-e] [-v] | -h\n\
\n\
Arguments:\n\
\t-f, --file\t\t\tRequired. Archive to sign.\n\
\t-p, --private-key\t\tRequired. Private key to sign. Can be used multiple times.\n\
\t-c, --certificate\t\tRequired. Certificate of signer. Can be used multiple times.\n\
\t-i, --intermediate\t\tAdd intermediate certificate.  Can be used multiple times.\n\
\t-e, --embed-certificate\t\tEmbed signers certificate in signature.\n\
\t-v, --verbose\t\t\tEnable additional output.\n\
\t-h, --help\t\t\tPrint usage.",
        VERSION
    );
}

pub fn print_verify_usage() {
    println!(
        "zipsign v{}, Copyright (c) 2019-2026 Falk Werner\n\
Signs and verifies ZIP archives\n\
\n\
verify: Verifies the signature of a zip archive.\n\
\n\
Usage:\n\
\tzipsign verify -f <value> -c <value> [-k <value>] [-v] [-s] | -h\n\
\n\
Arguments:\n\
\t-f, --file\t\t\tRequired. Archive to verify.\n\
\t-c, --certificate\t\tRequired. Certificate of signer. Can be used multiple times.\n\
\t-k, --keyring\t\t\tPath of keyring file.\n\
\t-v, --verbose\t\t\tEnable additionl output.\n\
\t-s, --self-signed\t\tAllows self signed certificates, skip cert verify.\n\
\t-h, --help\t\t\tPrint usage.",
        VERSION
    );
}

pub fn print_info_usage() {
    println!(
        "zipsign v{}, Copyright (c) 2019-2026 Falk Werner\n\
Signs and verifies ZIP archives\n\
\n\
info: Print info about the signature of zip archive.\n\
\n\
Usage:\n\
\tzipsign info -f <value> | -h\n\
\n\
Arguments:\n\
\t-f, --file\t\t\tRequired. Archive to verify.\n\
\t-h, --help\t\t\tPrint usage.",
        VERSION
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

