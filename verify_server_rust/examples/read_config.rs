#![allow(dead_code)]

use std::io::BufReader;

use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct Config {
    email: Email,
    mysql: Mysql,
    redis: Redis,
}

#[derive(Debug, Deserialize)]
pub struct Email {
    user: String,
    passwd: String,
}

#[derive(Debug, Deserialize)]
pub struct Mysql {
    host: String,
    port: usize,
    passwd: String,
}

#[derive(Debug, Deserialize)]
pub struct Redis {
    host: String,
    port: usize,
    passwd: String,
}
fn read_config() -> Result<Config, Box<dyn std::error::Error>> {
    let file = std::fs::File::open("../config.json")?;
    let file = BufReader::new(file);

    let config = serde_json::from_reader(file)?;

    Ok(config)
}

fn main() {
    let config = read_config().unwrap();
    println!("{:#?}", config);
}
