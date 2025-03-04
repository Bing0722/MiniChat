#pragma once

#include "CSession.h"
#include "Singleton.h"
#include "data.h"
#include "global.h"
#include <functional>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <map>
#include <queue>
#include <thread>
#include <unordered_map>

typedef std::function<void(std::shared_ptr<CSession>, const short &msg_id,
                           const std::string &msg_data)>
    FunCallBack;

class LogicSystem : public Singleton<LogicSystem> {
  friend class Singleton<LogicSystem>;

public:
  ~LogicSystem();
  void PostMsgToQue(std::shared_ptr<LogicNode> msg);

private:
  LogicSystem();

  void DealMsg();
  void RegisterCallBacks();
  void LoginHandler(std::shared_ptr<CSession> session, const short &msg_id,
                    const std::string &msg_data);
  void SearchInfo(std::shared_ptr<CSession> session, const short &msg_id,
                  const std::string &msg_data);

  void AddFriendApply(std::shared_ptr<CSession> session, const short &msg_id,
                      const std::string &msg_data);

  void AuthFriendApply(std::shared_ptr<CSession> session, const short &msg_id,
                       const std::string &msg_data);
  void DealChatTextMsg(std::shared_ptr<CSession> session, const short &msg_id,
                       const std::string &msg_data);

  bool IsPureDigit(const std::string &str);
  void GetUserByUID(std::string uid_str, Json::Value &rtvalue);
  void GetUserByName(std::string name, Json::Value &rtvalue);

  bool GetBaseInfo(std::string base_key, int uid,
                   std::shared_ptr<UserInfo> &userinfo);

  bool GetFriendApplyInfo(int to_uid,
                          std::vector<std::shared_ptr<ApplyInfo>> &list);

  bool GetFriendList(int self_id,
                     std::vector<std::shared_ptr<UserInfo>> &user_list);

  bool stop_;
  std::mutex mtx_;
  std::condition_variable cv_;
  std::thread worker_thread_;
  std::queue<std::shared_ptr<LogicNode>> msg_que_;
  std::map<short, FunCallBack> fun_callbacks_;
  // -------------------------
  std::map<int, std::shared_ptr<UserInfo>> users_;
};