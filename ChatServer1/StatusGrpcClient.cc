#include "StatusGrpcClient.h"
#include "ConfigMgr.h"

// 构造函数
StatusGrpcClient::StatusGrpcClient() {
  auto &config = ConfigMgr::Instance();
  std::string host = config["StatusServer"]["Host"];
  std::string port = config["StatusServer"]["Port"];

  pool_.reset(new StatusConPool(5, host, port));
}

// 根据 uid 来 获取 chat 服务响应
GetChatServerRsp StatusGrpcClient::GetChatServer(int uid) {

  ClientContext context;
  GetChatServerReq request;
  GetChatServerRsp response;

  request.set_uid(uid);

  auto stub = pool_->GetConnection();
  if (stub == nullptr) {
    response.set_error(ErrorCodes::RPC_FAILD);
    pool_->ReturnConnection(std::move(stub));
    return response;
  }

  Status status = stub->GetChatServer(&context, request, &response);
  if (status.ok()) {
    pool_->ReturnConnection(std::move(stub));
    return response;
  } else {
    response.set_error(ErrorCodes::RPC_FAILD);
    pool_->ReturnConnection(std::move(stub));
    return response;
  }
}

StatusGrpcClient::~StatusGrpcClient() {}

LoginRsp StatusGrpcClient::Login(int uid, std::string token) {
  ClientContext context;
  LoginReq request;
  LoginRsp response;

  request.set_uid(uid);
  request.set_token(token);

  auto stub = pool_->GetConnection();

  if (stub == nullptr) {
    response.set_error(ErrorCodes::RPC_FAILD);
    pool_->ReturnConnection(std::move(stub));
    return response;
  }

  Status status = stub->Login(&context, request, &response);
  if (status.ok()) {
    pool_->ReturnConnection(std::move(stub));
    return response;
  } else {
    response.set_error(ErrorCodes::RPC_FAILD);
    pool_->ReturnConnection(std::move(stub));
    return response;
  }
}

StatusConPool::StatusConPool(std::size_t poolsize, std::string host,
                             std::string port)
    : poolSize_(poolsize), host_(host), port_(port), stop_(false) {

  for (std::size_t i = 0; i < poolsize; ++i) {
    std::shared_ptr<Channel> channel = grpc::CreateChannel(
        host + ":" + port, grpc::InsecureChannelCredentials());

    connections_.push(StatusService::NewStub(channel));
  }
}

StatusConPool::~StatusConPool() {
  std::lock_guard<std::mutex> lock(mtx_);
  Close();
  while (!connections_.empty()) {
    connections_.pop();
  }
}

std::unique_ptr<StatusService::Stub> StatusConPool::GetConnection() {
  std::unique_lock<std::mutex> lock(mtx_);
  cv_.wait(lock, [this]() { return !connections_.empty() || stop_; });

  if (stop_) {
    return nullptr;
  }

  auto context = std::move(connections_.front());
  connections_.pop();
  return context;
}

void StatusConPool::ReturnConnection(
    std::unique_ptr<StatusService::Stub> context) {
  std::lock_guard<std::mutex> lock(mtx_);
  if (stop_) {
    return;
  }
  connections_.push(std::move(context));
  cv_.notify_one();
}

void StatusConPool::Close() {
  stop_ = true;
  cv_.notify_all();
}