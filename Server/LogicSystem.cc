#include "LogicSystem.h"
#include "HttpConnection.h"
#include <boost/beast/core/ostream.hpp>
#include <json/value.h>
#include <string>

// 注册一个GET请求
void LogicSystem::RegGet(std::string url, HttpHandler handler) {
  get_handlers_.insert(std::make_pair(url, handler));
}
// 注册一个POST请求
void LogicSystem::RegPost(std::string url, HttpHandler handler) {
  post_handlers_.insert(std::make_pair(url, handler));
}

LogicSystem::~LogicSystem() {}

LogicSystem::LogicSystem() {
  RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
    beast::ostream(connection->response_.body())
        << "receive get_test req" << std::endl;
    int i = 0;
    for (auto &elem : connection->get_params) {
      i++;
      beast::ostream(connection->response_.body())
          << "param " << i << " key is " << elem.first;
      beast::ostream(connection->response_.body())
          << "," << " value is " << elem.second << std::endl;
    }
  });

  RegPost("/get_varify_code", [](std::shared_ptr<HttpConnection> connection) {
    // 将请求体转换成字符串
    auto body_str =
        boost::beast::buffers_to_string(connection->request_.body().data());

    std::cout << "receive body is " << body_str << std::endl;

    connection->response_.set(http::field::content_type, "text/json");

    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;

    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
      std::cout << "Failed to parse JSON data!" << std::endl;
      root["error"] = ErrorCodes::ERROR_JSON;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    auto email = src_root["email"].asString();
    std::cout << "email is " << email << std::endl;
    root["error"] = 0;
    root["email"] = src_root["email"];
    std::string json_str = root.toStyledString();
    beast::ostream(connection->response_.body()) << json_str << std::endl;
    return true;
  });
}

bool LogicSystem::HandleGet(std::string path,
                            std::shared_ptr<HttpConnection> con) {
  if (get_handlers_.find(path) == get_handlers_.end()) {
    return false;
  }

  get_handlers_[path](con);
  return true;
}

bool LogicSystem::HandlePost(std::string path,
                             std::shared_ptr<HttpConnection> con) {

  if (post_handlers_.find(path) == post_handlers_.end()) {
    return false;
  }

  post_handlers_[path](con);
  return true;
}
