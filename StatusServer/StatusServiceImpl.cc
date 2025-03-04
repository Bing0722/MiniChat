#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "global.h"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <limits>

// 生成唯一的字符串 通常用作标识符
std::string generate_unique_string() {
  // 创建 UUID 对象
  boost::uuids::uuid uuid = boost::uuids::random_generator()();

  // 将 UUID 转换为字符串
  std::string unique_string = to_string(uuid);
  return unique_string;
}

// 实现 gRPC 接口 获取 Chat 服务器信息
Status StatusServiceImpl::GetChatServer(ServerContext *context,
                                        const GetChatServerReq *request,
                                        GetChatServerRsp *response) {

  std::string prefix("MiniChat status server has revceived: ");
  const auto &server = GetChatService();

  response->set_host(server.host);
  response->set_port(server.port);

  response->set_error(ErrorCodes::SUCCESS);
  response->set_token(generate_unique_string());
  InsertToken(request->uid(), response->token());
  return Status::OK;
}

// 实现 gRPC 接口 获取 登录验证服务
Status StatusServiceImpl::Login(ServerContext *context, const LoginReq *request,
                                LoginRsp *response) {
  auto uid = request->uid();
  auto token = request->token();

  std::string uid_str = std::to_string(uid);
  std::string token_key = USERTOKENPREFIX + uid_str;
  std::string token_value = "";
  bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
  if (success) {
    response->set_error(ErrorCodes::UID_INVALID);
    return Status::OK;
  }

  if (token_value != token) {
    response->set_error(ErrorCodes::TOKEN_INVALID);
    return Status::OK;
  }

  response->set_error(ErrorCodes::SUCCESS);
  response->set_uid(uid);
  response->set_token(token);
  return Status::OK;
}

void StatusServiceImpl::InsertToken(int uid, std::string token) {
  std::string uid_str = std::to_string(uid);
  std::string token_key = USERTOKENPREFIX + uid_str;
  // FIXME 这里看看能不能做一下 设置处理
  RedisMgr::GetInstance()->Set(token_key, token);
}

ChatServer StatusServiceImpl::GetChatService() {
  std::lock_guard<std::mutex> guard(server_mtx_);
  auto minServer = servers_.begin()->second;
  auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, minServer.name);
  if (count_str.empty()) {
    // 不存在则默认设置为最大
  } else {
    minServer.con_count = std::stoi(count_str);
  }

  // 使用范围基于for循环
  for (auto &server : servers_) {
    if (server.second.name == minServer.name) {
      continue;
    }

    auto count_str =
        RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server.second.name);
    if (count_str.empty()) {
      server.second.con_count = INT_MAX;
    } else {
      server.second.con_count = std::stoi(count_str);
    }

    if (server.second.con_count < minServer.con_count) {
      minServer = server.second;
    }
  }

  return minServer;
}

StatusServiceImpl::StatusServiceImpl() {
  auto &config = ConfigMgr::Instance();
  auto server_list = config["chatservers"]["Name"];

  std::vector<std::string> words;

  std::stringstream ss(server_list);
  std::string word;

  while (std::getline(ss, word, ',')) {
    words.push_back(word);
  }

  for (auto &word : words) {
    if (config[word]["Name"].empty()) {
      continue;
    }

    ChatServer server;
    server.port = config[word]["Post"];
    server.port = config[word]["Host"];
    server.port = config[word]["Name"];
    servers_[server.name] = server;
  }
}