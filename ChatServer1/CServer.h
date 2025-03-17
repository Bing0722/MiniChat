#pragma once

#include <boost/asio.hpp>
#include <map>
#include <memory>
#include <mutex>

#include "CSession.h"

using boost::asio::ip::tcp;

class CServer {
 public:
  CServer(boost::asio::io_context &ioc, short port);
  ~CServer();
  void ClearSession(std::string);

 private:
  void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code &error);

  void StartAccept();

  boost::asio::io_context &io_context_;
  short port_;
  tcp::acceptor acceptor_;
  std::map<std::string, std::shared_ptr<CSession>> session_;
  std::mutex mtx_;
};
