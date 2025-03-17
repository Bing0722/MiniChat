use tonic::transport::Server;
use verify_server_rust::{
    config::Config,
    proto::{VerifyServer, message::verify_service_server::VerifyServiceServer},
};

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let addr = "127.0.0.1:50051".parse()?;
    let config = Config::new("config.json");
    let server = VerifyServer::new("http://example.com", config);

    Server::builder()
        .add_service(VerifyServiceServer::new(server))
        .serve(addr)
        .await?;

    Ok(())
}
