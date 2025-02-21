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

using grpc::Channel;       // 与gRPC服务器连接的通道
using grpc::ClientContext; // 用于在客户端发起请求时的上下文
using grpc::Status;        // gRPC的状态

using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class RPConPool {
public:
  RPConPool(size_t poolsize, std::string host, std::string);
  ~RPConPool();

  std::unique_ptr<VerifyService::Stub> GetConnection();

  void ReturnConnection(std::unique_ptr<VerifyService::Stub> context);

  void Close();

private:
  std::atomic<bool> stop_;
  size_t poolsize_;
  std::string host_;
  std::string port_;
  std::queue<std::unique_ptr<VerifyService::Stub>> connections_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
  friend class Singleton<VerifyGrpcClient>;

public:
  // 客户端请求验证码的方法
  GetVerifyRsp GetVerifyCode(std::string email);

private:
  // 确保 VerifyGrpcClient类只能由单例创建
  VerifyGrpcClient();

  // gRPC客户端调用服务器端的服务接口
  // std::unique_ptr<VerifyService::Stub> stub_;

  std::unique_ptr<RPConPool> pool_;
};
