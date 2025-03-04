use reqwest::get;
use tokio;

async fn test() {
    let body = get("https://www.rust.lang.org").await;
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_url() {
        tokio::spawn(async {
            test().await;
        });
    }
}
