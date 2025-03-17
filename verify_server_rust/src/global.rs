#![allow(dead_code)]

pub const PREFIX: &str = "code_";

pub enum Errors {
    Success = 0,
    RedisError = 1,
    Exception = 2,
}
