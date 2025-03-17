use config::Config;
use dotenvy::dotenv;
use proto::{VerifyServer, start};
use tracing::info;
use tracing_subscriber::fmt::time;

mod config;
mod email;
mod global;
mod proto;
mod redis;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // 初始化日志
    tracing_subscriber::fmt()
        .with_timer(time::ChronoLocal::new("%Y-%m-%d %H:%M:%S".to_string()))
        .init();

    // 读取配置文件
    let config = Config::new("../config.json");
    info!("配置文件读取成功");

    // 读取数据库信息
    dotenv().ok();
    let url = std::env::var("REDIS_DATABASE_URL")
        .or_else(|_| std::env::var("REDIS_URL"))
        .expect("REDIS_URL 必须要被设置");

    // 创建验证服务器
    let verify_server = VerifyServer::new(&url, config);

    // 开启服务器
    start("127.0.0.1:50051", verify_server).await?;
    Ok(())
}
