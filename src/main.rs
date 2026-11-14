mod cli;
mod error;
mod ffi;
mod info;
mod signer;
mod verifier;
mod zip;

use signer::Signer;
use verifier::Verifier;

fn run_sign(args: &[String]) -> i32 {
    let sign_args = cli::parse_sign_args(args);

    if sign_args.help {
        cli::print_sign_usage();
        return 0;
    }
    if sign_args.unrecognized {
        eprintln!("error: unrecognized argument");
        cli::print_sign_usage();
        return 1;
    }
    if sign_args.file.is_none() {
        eprintln!("error: missig required argument: -f");
        cli::print_sign_usage();
        return 1;
    }
    if sign_args.keys.is_empty() {
        eprintln!("error: missig required argument: -p");
        cli::print_sign_usage();
        return 1;
    }
    if sign_args.certs.is_empty() {
        eprintln!("error: missig required argument: -c");
        cli::print_sign_usage();
        return 1;
    }
    if sign_args.keys.len() != sign_args.certs.len() {
        eprintln!("error: count of keys and signer certificated does not match");
        cli::print_sign_usage();
        return 1;
    }

    let mut signer = Signer::new(sign_args.embed_cert, sign_args.verbose);
    for inter in sign_args.intermediates {
        if let Err(e) = signer.add_intermediate(&inter) {
            eprintln!("error: {}", e);
            return 1;
        }
    }
    for (k, c) in sign_args.keys.iter().zip(sign_args.certs.iter()) {
        if let Err(e) = signer.add_signer(k, c) {
            eprintln!("error: {}", e);
            return 1;
        }
    }

    if let Err(e) = signer.sign(sign_args.file.as_ref().unwrap()) {
        eprintln!("error: {}", e);
        return 1;
    }

    0
}

fn run_verify(args: &[String]) -> i32 {
    let verify_args = cli::parse_verify_args(args);

    if verify_args.help {
        cli::print_verify_usage();
        return 0;
    }
    if verify_args.unrecognized {
        eprintln!("error: unrecognized argument");
        cli::print_verify_usage();
        return 1;
    }
    if verify_args.file.is_none() {
        eprintln!("error: missing required argument: -f");
        cli::print_verify_usage();
        return 1;
    }
    if verify_args.certs.is_empty() {
        eprintln!("error: missing required argument: -c");
        cli::print_verify_usage();
        return 1;
    }

    let mut verifier = Verifier::new();
    for c in &verify_args.certs {
        if let Err(e) = verifier.add_certificate(c) {
            eprintln!("error: {}", e);
            return 1;
        }
    }

    match verifier.verify(
        verify_args.file.as_ref().unwrap(),
        verify_args.keyring.as_deref(),
        verify_args.verbose,
        verify_args.self_signed,
    ) {
        Ok(result) => {
            println!("{}", result);
            result as i32
        }
        Err(e) => {
            eprintln!("error: {}", e);
            1
        }
    }
}

fn run_info(args: &[String]) -> i32 {
    let info_args = cli::parse_info_args(args);

    if info_args.help {
        cli::print_info_usage();
        return 0;
    }
    if info_args.unrecognized {
        eprintln!("error: unrecognized argument");
        cli::print_info_usage();
        return 1;
    }
    if info_args.file.is_none() {
        eprintln!("error: missig required argument: -f");
        cli::print_info_usage();
        return 1;
    }

    if let Err(e) = info::run_info(info_args.file.as_ref().unwrap()) {
        eprintln!("error: {}", e);
        return 1;
    }

    0
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() <= 1 {
        eprintln!("error: missing verb");
        cli::print_general_usage();
        std::process::exit(1);
    }

    let verb = &args[1];
    let exit_code = match verb.as_str() {
        "-h" | "--help" => {
            cli::print_general_usage();
            0
        }
        "-V" | "--version" => {
            println!("{}", cli::VERSION);
            0
        }
        "sign" => run_sign(&args[2..]),
        "verify" => run_verify(&args[2..]),
        "info" => run_info(&args[2..]),
        _ => {
            eprintln!("error: unknown verb: {}", verb);
            cli::print_general_usage();
            1
        }
    };

    std::process::exit(exit_code);
}







