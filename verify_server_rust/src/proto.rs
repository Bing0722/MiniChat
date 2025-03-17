use lettre::{
    AsyncSmtpTransport, AsyncTransport, Tokio1Executor,
    transport::smtp::authentication::Credentials,
};
use message::{
    GetVerifyReq, GetVerifyRsp,
    verify_service_server::{VerifyService, VerifyServiceServer},
};
use rand::seq::SliceRandom;
use redis::AsyncCommands;
use tonic::{Request, Response, Status, transport::Server};
use tracing::{debug, error, info, warn};

use crate::{config::Config, global::PREFIX};

pub mod message {
    tonic::include_proto!("message");
}

#[derive(Debug)]
pub struct VerifyServer {
    redis: redis::Client,
    emailer: AsyncSmtpTransport<Tokio1Executor>,
    config: Config,
}

impl VerifyServer {
    pub fn new(url: &str, config: Config) -> Self {
        let client = redis::Client::open(url).unwrap();

        let creds = Credentials::new(config.email.user.clone(), config.email.passwd.clone());

        let mailer = AsyncSmtpTransport::<Tokio1Executor>::relay("smtp.163.com")
            .unwrap()
            .port(465)
            .credentials(creds)
            .build();

        Self {
            redis: client,
            emailer: mailer,
            config,
        }
    }

    pub async fn send_email(&self, email: &str, code: &str) -> Result<(), ()> {
        let message = format!("您的验证码是: {}", code);

        let email = lettre::Message::builder()
            .from(self.config.email.user.parse().unwrap())
            .to(email.parse().unwrap())
            .subject("验证码")
            .body(message)
            .unwrap();

        let result = self.emailer.send(email).await;

        match result {
            Ok(_) => {
                info!("邮箱发送成功");
                Ok(())
            }
            Err(e) => {
                debug!("邮箱发送失败: {e}");
                Err(())
            }
        }
    }
}

#[tonic::async_trait]
impl VerifyService for VerifyServer {
    async fn get_verify_code(
        &self,
        request: Request<GetVerifyReq>,
    ) -> Result<Response<GetVerifyRsp>, Status> {
        let email = request.into_inner().email;
        println!("接收到的邮箱: {}", email);

        let mut reply = GetVerifyRsp {
            error: 0,
            email: email.clone(),
            code: "".to_owned(),
        };

        // 获取Redis连接
        let mut con = match self.redis.get_multiplexed_async_connection().await {
            Ok(con) => con,
            Err(e) => {
                error!("获取Redis连接失败: {}", e);
                reply.error = 1;
                return Ok(Response::new(reply));
            }
        };

        let code = generate_verification_code();
        let email_key = String::from(PREFIX) + &email;

        // 已发送验证码
        if let Ok(value) = con.get(&email_key).await {
            match value {
                Some(value) => {
                    let _: String = value;
                    warn!("邮箱已存在!");
                    reply.error = 2;
                    return Ok(Response::new(reply));
                }
                None => {}
            }
        }

        // 发送验证码
        if let Err(_) = self.send_email(email.as_str(), &code).await {
            error!("发送邮件失败");
            reply.error = 2;
            return Ok(Response::new(reply));
        }

        // 写入验证码到Redis
        if let Ok(()) = con.set_ex(&email_key, &code, 600).await {
            info!("redis 写入成功");
            return Ok(Response::new(reply));
        } else {
            reply.error = 1;
            error!("redis 写入失败");
            return Ok(Response::new(reply));
        }
    }
}

fn generate_verification_code() -> String {
    let mut uuid: Vec<char> = uuid::Uuid::new_v4()
        .to_string()
        .chars()
        .filter(|&c| c != '-')
        .collect();
    let mut rng = rand::rng();
    uuid.shuffle(&mut rng);
    let uuid = uuid.iter().take(6).collect();
    uuid
}

pub async fn start(addr: &str, server: VerifyServer) -> Result<(), Box<dyn std::error::Error>> {
    info!("启动验证服务...  监听: {}", addr);
    let addr = addr.parse()?;
    Server::builder()
        .add_service(VerifyServiceServer::new(server))
        .serve_with_shutdown(addr, async {
            tokio::signal::ctrl_c().await.expect("无法捕获 Ctrl+C");
            info!("gRPC服务器正在关闭...");
        })
        // .serve(addr)
        .await?;
    Ok(())
}
