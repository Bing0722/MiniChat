#include "LogicSystem.h"
#include "ConfigMgr.h"
#include "MysqlMgr.h"
#include "RedisMgr.h"
#include "StatusGrpcClient.h"
#include "UserMgr.h"

LogicSystem::LogicSystem() : stop_(false) {
  RegisterCallBacks();
  worker_thread_ = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem() {}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg) {}

void LogicSystem::DealMsg() {}

void LogicSystem::RegisterCallBacks() {
  fun_callbacks_[MSGIDS::MSG_CHAT_LOGIN] =
      std::bind(&LogicSystem::LoginHandler, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::LoginHandler(std::shared_ptr<CSession> session,
                               const short &msg_id,
                               const std::string &msg_data) {
  Json::Reader reader;
  Json::Value root;
  reader.parse(msg_data, root);
  auto uid = root["uid"].asInt();
  auto token = root["token"].asString();
  std::cout << "user login uid is " << uid << " user token is "
            << root["token"].asString() << std::endl;

  // ------------------------------------------------------
  auto rsp =
      StatusGrpcClient::GetInstance()->Login(uid, root["token"].asString());
  Json::Value rtvalue;

  rtvalue["error"] = rsp.error();
  if (rsp.error() != ErrorCodes::SUCCESS) {
    return;
  }

  auto find_iter = users_.find(uid);
  std::shared_ptr<UserInfo> user_info = nullptr;
  if (find_iter == users_.end()) {
    user_info = MysqlMgr::GetInstance()->GetUser(uid);
    if (user_info == nullptr) {
      rtvalue["error"] = ErrorCodes::UID_INVALID;
      return;
    }

    users_[uid] = user_info;

  } else {
    user_info = find_iter->second;
  }

  rtvalue["uid"] = uid;
  rtvalue["token"] = rsp.token();
  rtvalue["name"] = user_info->name;

  //-------------------------------------------------------

  // //  FIXME
  // Json::Value rtvalue;

  // // 从Redis获取用户token是否正确
  // std::string uid_str = std::to_string(uid);
  // std::string token_key = USERTOKENPREFIX + uid_str;
  // std::string token_value = "";
  // bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
  // if (!success) {
  //   rtvalue["error"] = ErrorCodes::UID_INVALID;
  //   return;
  // }

  // if (token_value != token) {
  //   rtvalue["error"] = ErrorCodes::TOKEN_INVALID;
  //   return;
  // }

  // rtvalue["error"] = ErrorCodes::SUCCESS;

  // std::string base_key = USER_BASE_INFO + uid_str;
  // auto user_info = std::make_shared<UserInfo>();
  // bool b_base = GetBaseInfo(base_key, uid, user_info);
  // if (!b_base) {
  //   rtvalue["error"] = ErrorCodes::UID_INVALID;
  //   return;
  // }
  // rtvalue["uid"] = uid;
  // rtvalue["pass"] = user_info->pass;
  // rtvalue["name"] = user_info->name;
  // rtvalue["email"] = user_info->email;
  // rtvalue["nick"] = user_info->nick;
  // rtvalue["desc"] = user_info->desc;
  // rtvalue["sex"] = user_info->sex;
  // rtvalue["info"] = user_info->icon;

  // // 从数据库获取申请列表
  // std::vector<std::shared_ptr<ApplyInfo>> apply_list;
  // auto b_apply = GetFriendApplyInfo(uid, apply_list);
  // if (b_apply) {
  //   for (auto &apply : apply_list) {
  //     Json::Value obj;
  //     obj["name"] = apply->name_;
  //     obj["uid"] = apply->uid_;
  //     obj["icon"] = apply->icon_;
  //     obj["nick"] = apply->nick_;
  //     obj["sex"] = apply->sex_;
  //     obj["desc"] = apply->desc_;
  //     obj["status"] = apply->desc_;
  //     rtvalue["apply_list"].append(obj);
  //   }
  // }

  // // 获取好友列表
  // std::vector<std::shared_ptr<UserInfo>> friend_list;
  // bool b_friend_list = GetFriendList(uid, friend_list);
  // for (auto &friend_ele : friend_list) {
  //   Json::Value obj;
  //   obj["name"] = friend_ele->name;
  //   obj["uid"] = friend_ele->uid;
  //   obj["icon"] = friend_ele->icon;
  //   obj["nick"] = friend_ele->nick;
  //   obj["sex"] = friend_ele->sex;
  //   obj["desc"] = friend_ele->desc;
  //   obj["back"] = friend_ele->back;
  //   rtvalue["friend_list"].append(obj);
  // }

  // // TODO  ConfigMgr
  // // auto server_name = ConfigMgr::Instance();
  // std::string server_name;
  // // 将登录数增加
  // auto redis_res = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server_name);
  // int count = 0;
  // if (!redis_res.empty()) {
  //   count = std::stoi(redis_res);
  // }

  // count++;
  // auto count_str = std::to_string(count);
  // RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, count_str);
  // session->SetUserID(uid);
  // std::string ipkey = USERIPPREFIX + uid_str;
  // RedisMgr::GetInstance()->Set(ipkey, server_name);
  // UserMgr::GetInstance()->SetUserSession(uid, session);
  // return;
}

void LogicSystem::SearchInfo(std::shared_ptr<CSession> session,
                             const short &msg_id, const std::string &msg_data) {

}

void LogicSystem::AddFriendApply(std::shared_ptr<CSession> session,
                                 const short &msg_id,
                                 const std::string &msg_data) {}

void LogicSystem::AuthFriendApply(std::shared_ptr<CSession> session,
                                  const short &msg_id,
                                  const std::string &msg_data) {}
void LogicSystem::DealChatTextMsg(std::shared_ptr<CSession> session,
                                  const short &msg_id,
                                  const std::string &msg_data) {}

bool LogicSystem::IsPureDigit(const std::string &str) { return false; }

void LogicSystem::GetUserByUID(std::string uid_str, Json::Value &rtvalue) {}

void LogicSystem::GetUserByName(std::string name, Json::Value &rtvalue) {}

bool LogicSystem::GetBaseInfo(std::string base_key, int uid,
                              std::shared_ptr<UserInfo> &userinfo) {
  return false;
}

bool LogicSystem::GetFriendApplyInfo(
    int to_uid, std::vector<std::shared_ptr<ApplyInfo>> &list) {
  return false;
}

bool LogicSystem::GetFriendList(
    int self_id, std::vector<std::shared_ptr<UserInfo>> &user_list) {
  return false;
}