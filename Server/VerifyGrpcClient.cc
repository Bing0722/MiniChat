#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"

RPConPool::RPConPool(size_t poolsize, std::string host, std::string port)
    : poolsize_(poolsize), host_(host), port_(port), stop_(false) {
  for (size_t i = 0; i < poolsize; i++) {
    std::shared_ptr<Channel> channel = grpc::CreateChannel(
        host + ":" + port, grpc::InsecureChannelCredentials());

    connections_.push(VerifyService::NewStub(channel));
  }
}

RPConPool::~RPConPool() {
  std::lock_guard<std::mutex> lock(mutex_);
  Close();
  while (!connections_.empty()) {
    connections_.pop();
  }
}

std::unique_ptr<VerifyService::Stub> RPConPool::GetConnection() {
  std::unique_lock<std::mutex> lock(mutex_);
  // 当线程进入时等待 直到返回 true
  cond_.wait(lock, [this]() {
    if (stop_) {
      return true;
    }
    return !connections_.empty();
  });

  if (stop_) {
    return nullptr;
  }

  auto context = std::move(connections_.front());
  connections_.pop();
  return context;
}

void RPConPool::ReturnConnection(std::unique_ptr<VerifyService::Stub> context) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (stop_) {
    return;
  }
  connections_.push(std::move(context));
  cond_.notify_one();
}

void RPConPool::Close() {
  stop_ = true;
  cond_.notify_all();
}

VerifyGrpcClient::VerifyGrpcClient() {
  auto &cfg_mgr = ConfigMgr::Instance();
  std::string host = cfg_mgr["VerifyServer"]["Host"];
  std::string port = cfg_mgr["VerifyServer"]["Port"];
  // reset 方法 释放原来管理的对象, 并开始管理传入的新对象
  pool_.reset(new RPConPool(5, host, port));

  // 创建一个本地运行的gRPC服务端口为50051连接不使用加密
  // std::shared_ptr<Channel> channel = grpc::CreateChannel(
  //     "127.0.0.1:50051", grpc::InsecureChannelCredentials());

  // 创建一个客户端存根用来调用 VerifyService服务方法
  // stub_ = VerifyService::NewStub(channel);
}

GetVerifyRsp VerifyGrpcClient::GetVerifyCode(std::string email) {
  ClientContext context;
  GetVerifyRsp reply;
  GetVerifyReq request;
  request.set_email(email);

  // 从连接池中取出对象
  auto stub = pool_->GetConnection();

  Status status = stub->GetVerifyCode(&context, request, &reply);

  if (status.ok()) {
    // 返回对象到连接池
    pool_->ReturnConnection(std::move(stub));
    return reply;

  } else {
    pool_->ReturnConnection(std::move(stub));
    reply.set_error(ErrorCodes::RPC_FAILD);
    return reply;
  }
}
