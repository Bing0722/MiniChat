#include "UserMgr.h"
#include "CSession.h"
#include "RedisMgr.h"

UserMgr::~UserMgr() { uid_to_session_.clear(); }

std::shared_ptr<CSession> UserMgr::GetSession(int uid) {
  std::lock_guard<std::mutex> lock(session_mtx_);
  auto iter = uid_to_session_.find(uid);
  if (iter == uid_to_session_.end()) {
    return nullptr;
  }

  return iter->second;
}

void UserMgr::SetUserSession(int uid, std::shared_ptr<CSession> session) {
  std::lock_guard<std::mutex> lock(session_mtx_);
  uid_to_session_[uid] = session;
}

void UserMgr::RemoveUserSession(int uid) {
  auto uid_str = std::to_string(uid);

  {
    std::lock_guard<std::mutex> lock(session_mtx_);
    uid_to_session_.erase(uid);
  }
}

UserMgr::UserMgr() {}