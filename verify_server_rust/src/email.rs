//! Email module
#![allow(dead_code)]

use lettre::{
    AsyncSmtpTransport, AsyncTransport, Message, Tokio1Executor,
    transport::smtp::authentication::Credentials,
};

use crate::config::Config;

fn generate_verification_code() -> String {
    // Generate a random verification code
    let code = rand::random_range(100000..1000000);
    code.to_string()
}

pub async fn send_email(to_email: &str, config: &Config) {
    let verify_code = generate_verification_code();

    let email = Message::builder()
        .from(config.email.user.parse().unwrap())
        .to(to_email.parse().unwrap())
        .subject("Verification Code")
        .body(format!("Your verification code is: {}", verify_code))
        .unwrap();

    let creds = Credentials::new(config.email.user.clone(), config.email.passwd.clone());

    let mailer = AsyncSmtpTransport::<Tokio1Executor>::relay("smtp.163.com")
        .unwrap()
        .port(465)
        .credentials(creds)
        .build();

    match mailer.send(email).await {
        Ok(_) => println!("邮件发送成功"),
        Err(e) => println!("Failed to send email: {}", e),
    }
}
