#include "CServer.h"
#include "AsioIOServicePool.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short &port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc) {
  std::cout << "Server started listening on port " << port << "..."
            << std::endl;
}

void CServer::start() {
  auto self = shared_from_this();

  auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();

  std::shared_ptr<HttpConnection> new_con =
      std::make_shared<HttpConnection>(io_context);

  acceptor_.async_accept(
      new_con->GetSocket(), [self, new_con](beast::error_code ec) {
        try {
          // 出错则放弃这个连接, 继续监听新连接
          if (ec) {
            // 继续监听
            self->start();
            return;
          }

          // 处理新连接, 创建HttpConnection 类管理新连接
          // 当 socket_传给 HttpConnection后 CServer 就会创建新的 socket_
          // std::make_shared<HttpConnection>(std::move(self->socket_))->start();

          new_con->start();

          // 继续监听
          self->start();
        } catch (std::exception &e) {
          std::cerr << "Exception is " << e.what() << std::endl;
          self->start();
        }
      });
}
