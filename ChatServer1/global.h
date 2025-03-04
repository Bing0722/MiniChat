#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <memory>
#include <queue>
#include <unordered_map>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
  SUCCESS = 0,
  ERROR_JSON = 1001,        // json解析错误
  RPC_FAILD = 1002,         // RPC请求错误
  VERIFY_EXPIRED = 1003,    // 验证码过期
  VERIFY_CODE_ERR = 1004,   // 验证码错误
  USER_EXIST = 1005,        // 用户已存在
  PASSWORD_ERR = 1006,      // 密码错误
  EMAIL_NOT_MATCH = 1007,   // 邮箱不匹配
  PASSWD_UPDATE_ERR = 1008, // 更新密码失败
  TOKEN_INVALID = 1009,     // token失效
  UID_INVALID = 1010,       // UID无效
};

#define MAX_LENGTH 1024 * 2
// 头部总长度
#define HEAD_TOTAL_LEN 4
// 头部id长度
#define HEAD_ID_LEN 2
// 头部数据长度
#define HEAD_DATA_LEN 2
#define MAX_RECVQUE 10000
#define MAX_SENDQUE 1000

#define USERIPPREFIX "uip_"
#define USERTOKENPREFIX "utoken_"
#define IPCOUNTPREFIX "ipcount_"
#define USER_BASE_INFO "ubaseinfo_"
#define LOGIN_COUNT "logincount"
#define NAME_INFO "nameinfo_"

enum MSGIDS {
  MSG_CHAT_LOGIN = 1005, // 用户登录
};