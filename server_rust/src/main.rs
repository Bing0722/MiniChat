use utils::read_ini::*;

fn main() {
    let config = ConfigMgr::new();

    let s = &config.config.lock().unwrap()["GateServer"]["Host"];

    println!("Host: {}", s);
}
