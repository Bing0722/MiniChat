use verify_server_rust::config;
use verify_server_rust::email;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let config = config::load_config("../config.json")?;
    email::send_email("1693763287@qq.com", &config).await;
    println!("config: {:?}", config);
    Ok(())
}
