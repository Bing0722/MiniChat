#pragma once

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

#include "MsgNode.h"
#include "global.h"

class CServer;
class LogicSystem;

using tcp = boost::asio::ip::tcp;

class CSession : public std::enable_shared_from_this<CSession> {
 public:
  CSession(boost::asio::io_context &ioc, CServer *server);
  ~CSession();

  tcp::socket &GetSocket();
  std::string &GetSessionID();

  void Send(char *msg, short max_len, short msgid);
  void Send(std::string msg, short msgid);

  void SetUserID(int uid);
  int GetUserID();

  std::shared_ptr<CSession> SharedSelf();

  void Start();
  void Close();

  void AsyncReadBody(int total_len);
  void AsyncReadHead(int total_len);

 private:
  void AsyncReadFull(std::size_t max_len,
                     std::function<void(const boost::system::error_code &, std::size_t)> handler);

  void AsyncReadLen(std::size_t read_len, std::size_t total_len,
                    std::function<void(const boost::system::error_code &, std::size_t)> handler);

  void HandleWrite(const boost::system::error_code &error, std::shared_ptr<CSession> shared_self);

  char data_[MAX_LENGTH];
  tcp::socket socket_;
  std::string session_id_;

  CServer *server_;
  bool stop_;  // 服务是否要停止

  std::queue<std::shared_ptr<SendNode>> send_que_;
  std::mutex send_lock_;
  // 收到消息的结构
  std::shared_ptr<RecvNode> recv_msg_node_;
  bool b_head_parse_;  // 解析头是否完成
  // 收到头部的结构
  std::shared_ptr<MsgNode> recv_head_node_;

  int user_uid_;
};

class LogicNode {
  friend class LogicSystem;

 public:
  LogicNode(std::shared_ptr<CSession>, std::shared_ptr<RecvNode>);

 private:
  std::shared_ptr<CSession> session_;
  std::shared_ptr<RecvNode> recvnode_;
};
