//! redis module
#![allow(dead_code)]
use redis::{AsyncCommands, Commands, Connection};

pub struct Redis {
    pub conn: Connection,
    pub async_conn: redis::aio::MultiplexedConnection,
}

impl Redis {
    pub async fn new(url: String) -> Self {
        let client = redis::Client::open(url).unwrap();
        let conn = client.get_connection().unwrap();
        let async_conn = client.get_multiplexed_async_connection().await.unwrap();
        Self { conn, async_conn }
    }

    pub fn set_ex(&mut self, key: &str, value: &str, expire: u64) {
        match self.conn.set_ex(key, value, expire) {
            Ok(()) => println!("Key set successfully"),
            Err(e) => println!("Error setting key: {}", e),
        }
    }

    pub async fn get(&mut self, key: &str) -> Option<String> {
        match self.async_conn.get(key).await {
            Ok(value) => Some(value),
            Err(e) => {
                println!("Error getting key: {}", e);
                None
            }
        }
    }

    pub fn return_client(url: String) -> redis::Client {
        redis::Client::open(url).unwrap()
    }
}
