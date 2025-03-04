#include "CServer.h"
#include "AsioIOServicePool.h"
#include "UserMgr.h"
#include <iostream>

CServer::CServer(boost::asio::io_context &ioc, short port)
    : io_context_(ioc), port_(port),
      acceptor_(ioc, tcp::endpoint(tcp::v4(), port)) {

  std::cout << "Server start success, listening on port: " << port_
            << std::endl;
  // 开始处理接收
  StartAccept();
}

CServer::~CServer() {
  std::cout << "Server destruct listen on port: " << port_ << std::endl;
}

void CServer::StartAccept() {
  auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();

  std::shared_ptr<CSession> new_session =
      std::make_shared<CSession>(io_context, this);

  acceptor_.async_accept(new_session->GetSocket(),
                         std::bind(&CServer::HandleAccept, this, new_session,
                                   std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session,
                           const boost::system::error_code &error) {
  if (!error) {
    new_session->Start();
    std::lock_guard<std::mutex> lock(mtx_);
    session_.insert(std::make_pair(new_session->GetSessionID(), new_session));
  } else {
    std::cout << "session accept failed, error is " << error.what()
              << std::endl;
  }

  StartAccept();
}

void CServer::ClearSession(std::string uuid) {
  if (session_.find(uuid) != session_.end()) {
    UserMgr::GetInstance()->RemoveUserSession(session_[uuid]->GetUserID());
  }

  {
    std::lock_guard<std::mutex> lock(mtx_);
    session_.erase(uuid);
  }
}