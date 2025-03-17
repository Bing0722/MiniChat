#![allow(dead_code)]
use std::{fs::File, io::BufReader, path::Path};

use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct Config {
    pub email: Email,
    pub mysql: Mysql,
    pub redis: Redis,
}

#[derive(Debug, Deserialize)]
pub struct Email {
    pub user: String,
    pub passwd: String,
}

#[derive(Debug, Deserialize)]
pub struct Mysql {
    pub host: String,
    pub port: usize,
    pub passwd: String,
}

#[derive(Debug, Deserialize)]
pub struct Redis {
    pub host: String,
    pub port: usize,
    pub passwd: String,
}

impl Config {
    // 读取配置文件
    pub fn new<P: AsRef<Path>>(path: P) -> Self {
        let config_file = File::open(path).expect("Failed to open config file");
        let file = BufReader::new(config_file);
        let config: Config = serde_json::from_reader(file).expect("Failed to parse config file");
        config
    }
}

// 读取配置文件
pub fn load_config<P: AsRef<Path>>(path: P) -> Result<Config, Box<dyn std::error::Error>> {
    let config_file = File::open(path)?;
    let file = BufReader::new(config_file);
    let config: Config = serde_json::from_reader(file)?;
    Ok(config)
}
