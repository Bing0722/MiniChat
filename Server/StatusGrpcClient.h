#pragma once

#ifdef _WIN32
#include "./WIN32/message.grpc.pb.h"
#include "./WIN32/message.pb.h"
#elif __linux__
#include "./LINUX/message.grpc.pb.h"
#include "./LINUX/message.pb.h"
#endif

#include "Singleton.h"
#include "global.h"
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;

class StatusConPool;

class StatusGrpcClient : public Singleton<StatusGrpcClient> {
  friend class Singleton<StatusGrpcClient>;

public:
  ~StatusGrpcClient();

  // 根据 uid 来 获取 chat 服务响应
  GetChatServerRsp GetChatServer(int uid);

  StatusGrpcClient();

private:
  std::unique_ptr<StatusConPool> pool_;
};

// StatusConPool 连接池
class StatusConPool {
public:
  StatusConPool(std::size_t poolsize, std::string host, std::string port);
  ~StatusConPool();

  // 获取一个连接
  std::unique_ptr<StatusService::Stub> GetConnection();

  // 返回一个连接
  void ReturnConnection(std::unique_ptr<StatusService::Stub> context);

  void Close();

private:
  std::atomic<bool> stop_; // 连接池的停止标志位
  std::size_t poolSize_;
  std::string host_;
  std::string port_;
  std::queue<std::unique_ptr<StatusService::Stub>> connections_;
  std::mutex mtx_;
  std::condition_variable cv_;
};