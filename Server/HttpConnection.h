#pragma once

#include "global.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
  // LogicSystem 类可以访问 HttpConnection的私有成员和方法
  friend class LogicSystem;

public:
  // HttpConnection(tcp::socket socket);
  HttpConnection(boost::asio::io_context &ioc);

  // 启动http连接的处理过程
  void Start();

  tcp::socket &GetSocket();

private:
  // 检查处理请求的超时情况
  void CheckDeadline();
  // 将Http响应写回客户端
  void WriteResponse();
  // 处理接收到的Http请求
  void HandleReq();
  // url参数解析
  void PreParseGetParam();

  tcp::socket socket_;

  // 接收数据
  beast::flat_buffer buffer_{8192};

  // 解析请求
  http::request<http::dynamic_body> request_;

  // 回应客户端
  http::response<http::dynamic_body> response_;

  // 定时器判断请求是否超时
  // 创建时开始启动
  net::steady_timer deadline_{socket_.get_executor(), std::chrono::seconds(60)};

  // 获取 url
  std::string get_url_;
  std::unordered_map<std::string, std::string> get_params_;
};

extern unsigned char ToHex(unsigned char x);

extern unsigned char FromHex(unsigned char x);

// Url 编码
extern std::string UrlEncode(const std::string &str);

// url 解码
extern std::string UrlDecode(const std::string &str);
