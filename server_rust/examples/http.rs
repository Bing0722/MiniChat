use reqwest;
use tokio;

#[tokio::main]
async fn main() {
    let body = reqwest::get("https://github.com").await;
    if let Ok(body) = body {
        if let Ok(body) = body.text().await {
            println!("body = {body:?}");
        } else {
            panic!("err2");
        }
    } else {
        panic!("err1");
    }
}
