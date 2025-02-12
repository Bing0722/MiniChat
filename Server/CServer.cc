#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short &port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc) {}

void CServer::start() {
  auto self = shared_from_this();
  acceptor_.async_accept(socket_, [self](beast::error_code ec) {
    try {
      // 出错则放弃这个连接, 继续监听新连接
      if (ec) {
        // 继续监听
        self->start();
        return;
      }

      // 处理新连接, 创建HttpConnection 类管理新连接
      std::make_shared<HttpConnection>(std::move(self->socket_))->start();

      // 继续监听
      self->start();
    } catch (std::exception &e) {
      std::cerr << "Exception is " << e.what() << std::endl;
      self->start();
    }
  });
}
