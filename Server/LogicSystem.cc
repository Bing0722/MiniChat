#include "LogicSystem.h"
#include "HttpConnection.h"
#include "MysqlMgr.h"
#include "RedisMgr.h"
#include "VerifyGrpcClient.h"

// 注册一个GET请求
void LogicSystem::RegGet(std::string url, HttpHandler handler) {
  get_handlers_.insert(std::make_pair(url, handler));
}
// 注册一个POST请求
void LogicSystem::RegPost(std::string url, HttpHandler handler) {
  post_handlers_.insert(std::make_pair(url, handler));
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

LogicSystem::~LogicSystem() {}

LogicSystem::LogicSystem() {
  RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
    beast::ostream(connection->response_.body())
        << "receive get_test req" << std::endl;
    int i = 0;
    for (auto &elem : connection->get_params_) {
      i++;
      beast::ostream(connection->response_.body())
          << "param " << i << ": key is " << elem.first << "," << "value is "
          << elem.second << std::endl;
    }
  });

  // POST 获取验证码
  RegPost("/get_verify_code", [](std::shared_ptr<HttpConnection> connection) {
    // 将请求体转换成字符串
    auto body_str =
        boost::beast::buffers_to_string(connection->request_.body().data());

    std::cout << "receive body is " << body_str << std::endl;

    // 设置响应内容
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
    // 通过 gRPC 调用验证服务获取验证码
    GetVerifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVerifyCode(email);
    std::cout << "email is " << email << std::endl;
    root["error"] = rsp.error();
    root["email"] = src_root["email"];
    std::string json_str = root.toStyledString();
    beast::ostream(connection->response_.body()) << json_str << std::endl;
    return true;
  });

  // POST  注册用户请求
  RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection) {
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
    auto name = src_root["user"].asString();
    auto pass = src_root["pass"].asString();
    auto confirm = src_root["confirm"].asString();

    if (pass != confirm) {
      std::cout << "password err" << std::endl;
      root["error"] = ErrorCodes::PASSWORD_ERR;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    // 查找redis 中email对应的验证码是否合理
    std::string verify_code;

    bool get_verify = RedisMgr::GetInstance()->Get(
        CODEPREFIX + src_root["email"].asString(), verify_code);
    if (!get_verify) {
      std::cout << "get verify code expired!" << std::endl;
      root["error"] = ErrorCodes::VERIFY_EXPIRED;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    if (verify_code != src_root["verify_code"].asString()) {
      std::cout << "verify code error!" << std::endl;
      root["error"] = ErrorCodes::VERIFY_CODE_ERR;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    // 查找数据库判断用户是否存在
    int uid = MysqlMgr::GetInstance()->RegUser(name, email, pass);
    if (uid == 0 || uid == -1) {
      std::cout << "user or email exist" << std::endl;
      root["error"] = ErrorCodes::USER_EXIST;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    // 返回用户信息
    root["error"] = 0;
    root["uid"] = uid;
    root["email"] = email;
    root["user"] = name;
    root["pass"] = pass;
    root["confirm"] = confirm;
    root["verify_code"] = src_root["verify_code"];
    std::string json_str = root.toStyledString();
    beast::ostream(connection->response_.body()) << json_str << std::endl;
    return true;
  });

  // 重置密码逻辑
  RegPost("/reset_passwd", [](std::shared_ptr<HttpConnection> connection) {
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
    auto name = src_root["user"].asString();
    auto pass = src_root["pass"].asString();
    auto confirm = src_root["confirm"].asString();

    // 查找redis中的email对应的验证码是否合理
    std::string verify_code;
    bool b_get_verify = RedisMgr::GetInstance()->Get(
        CODEPREFIX + src_root["email"].asString(), verify_code);

    if (!b_get_verify) {
      std::cout << "Get verify code expired!" << std::endl;
      root["error"] = ErrorCodes::VERIFY_EXPIRED;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    if (verify_code != src_root["verify_code"].asString()) {
      std::cout << "Verify code error!" << std::endl;
      root["error"] = ErrorCodes::VERIFY_CODE_ERR;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    // 查询数据库判断用户名，和邮箱是否匹配
    bool email_valid = MysqlMgr::GetInstance()->CheckEmail(name, email);
    if (!email_valid) {
      std::cout << "User emaik not match!" << std::endl;
      root["error"] = ErrorCodes::EMAIL_NOT_MATCH;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    // 更新密码为最新密码
    bool b_update = MysqlMgr::GetInstance()->UpdatePasswd(name, pass);
    if (!b_update) {
      std::cout << "Update passwd failed!" << std::endl;
      root["error"] = ErrorCodes::PASSWD_UPDATE_ERR;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    std::cout << "Succeed to update passwd." << std::endl;
    root["error"] = 0;
    root["email"] = email;
    root["user"] = name;
    root["pass"] = pass;
    root["confirm"] = confirm;
    root["verify_code"] = src_root["verify_code"];
    std::string json_str = root.toStyledString();
    beast::ostream(connection->response_.body()) << json_str << std::endl;
    return true;
  });

  RegPost("/user_login", [](std::shared_ptr<HttpConnection> connection) {
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

    auto name = src_root["user"].asString();
    auto pass = src_root["pass"].asString();

    UserInfo userinfo;

    bool pass_valid =
        MysqlMgr::GetInstance()->CheckPasswd(name, pass, userinfo);

    if (!pass_valid) {
      std::cout << "User passwd not match!" << std::endl;
      root["error"] = ErrorCodes::PASSWORD_ERR;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    // TODO StatusServer 服务
    // 查询 StatusServer 找到合适的连接
    auto reply = StatusGrpcClient::GetInstance()->GetChatServer(userinfo, uid);
    if (reply.error()) {
      std::cout << "Grpc get chat server failed, error is " << reply.error()
                << std::endl;
      root["error"] = ErrorCodes::RPC_FAILD;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str << std::endl;
      return true;
    }

    std::cout << "Succeed to load user info uid is " << userinfo.uid
              << std::endl;
    root["error"] = 0;
    root["user"] = name;
    root["uid"] = userinfo.uid;
    root["token"] = userinfo.token();
    root["host"] = userinfo.host();
    std::string json_str = root.toStyledString();
    beast::ostream(connection->response_.body()) << json_str << std::endl;
    return true;
  });
}
