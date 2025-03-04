//! 用来读取 config.ini文件
//!
//!

use ini::Ini;
use std::{
    env,
    sync::{Arc, Mutex},
};

pub struct ConfigMgr {
    pub config: Arc<Mutex<Ini>>,
}

impl ConfigMgr {
    pub fn new() -> Self {
        // 读取当前文件位置
        let path = env::current_dir().expect("failed to get current dir.");
        // 拼接文件
        let config_file = path.join("./config.ini");

        println!("current dir: {config_file:?}");

        // 读取配置文件
        if let Ok(config) = Ini::load_from_file(config_file) {
            ConfigMgr {
                config: Arc::new(Mutex::new(config)),
            }
        } else {
            panic!("配置文件读取失败, 请重启！");
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_config_mgr() {
        let config_mgr = ConfigMgr::new();
        let s = &config_mgr.config.lock().unwrap()["GateServer"];

        for (k, v) in s {
            println!("{}:{}", k, v);
        }
    }
}
