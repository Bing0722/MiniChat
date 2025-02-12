#pragma once

#include "global.h"

class CServer : public std::enable_shared_from_this<CServer> {
public:
  CServer(boost::asio::io_context &ioc, unsigned short &port);

  // 启动服务器
  void start();

private:
  tcp::acceptor acceptor_;
  net::io_context &ioc_; // 引用外部传入的 io_context对象
  tcp::socket socket_;   // 套接字
};
