#pragma once

#include "MysqlDao.h"
#include "Singleton.h"

class MysqlMgr : public Singleton<MysqlMgr> {
  friend class Singleton<MysqlMgr>;

public:
  ~MysqlMgr();
  int RegUser(const std::string &name, const std::string &email,
              const std::string &pass);

  bool CheckEmail(const std::string &name, const std::string &email);
  bool UpdatePasswd(const std::string &name, const std::string &pass);
  bool CheckPasswd(const std::string &name, const std::string &pass,
                   UserInfo &userinfo);

private:
  MysqlMgr();
  MysqlDao dao_;
};