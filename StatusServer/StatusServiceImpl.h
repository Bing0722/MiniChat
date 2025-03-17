#pragma once

#ifdef _WIN32
#include "./WIN32/message.grpc.pb.h"
#include "./WIN32/message.pb.h"
#elif __linux__
#include "./LINUX/message.grpc.pb.h"
#include "./LINUX/message.pb.h"
#endif

#include <grpcpp/grpcpp.h>

using grpc::Server;        // gRPC 服务器类
using grpc::ServerBuilder; // gRPC 服务器构建器
using grpc::ServerContext; // gRPC 服务器上下文
using grpc::Status;        // gRPC 状态类

using message::GetChatServerReq; // 引入 GetChatServerReq 消息类型
using message::GetChatServerRsp; // 引入 GetChatServerRsp 消息类型
using message::LoginReq;         // 引入 LoginReq 消息类型
using message::LoginRsp;         // 引入 LoginRsp 消息类型
using message::StatusService;    // 引入 StatusService 服务类型

// 存储聊天服务器的结构体
class ChatServer {
public:
  ChatServer() : host(""), port(""), name(""), con_count(0) {}
  ChatServer(const ChatServer &cs)
      : host(cs.host), port(cs.port), name(cs.name), con_count(cs.con_count) {}
  ChatServer &operator=(const ChatServer &cs) {
    if (&cs == this) {
      return *this;
    }
    host = cs.host;
    name = cs.name;
    port = cs.port;
    con_count = cs.con_count;
    return *this;
  }
  std::string host;
  std::string port;
  std::string name;
  int con_count;
};

// StatusServiceImpl 类 继承自 StatusService::Service 类 并实现其接口
class StatusServiceImpl final : public StatusService::Service {
public:
  StatusServiceImpl();

  // 实现 gRPC 接口 获取 Chat 服务器信息
  Status GetChatServer(ServerContext *context, const GetChatServerReq *request,
                       GetChatServerRsp *response) override;

  Status Login(ServerContext *context, const LoginReq *request,
               LoginRsp *response) override;

private:
  void InsertToken(int uid, std::string token);
  ChatServer GetChatService();
  std::unordered_map<std::string, ChatServer> servers_;
  std::mutex server_mtx_;
  // std::vector<ChatServer> servers_; // 存储聊天服务器的信息
  // int server_index_; // 当前选择的服务器索引
};
