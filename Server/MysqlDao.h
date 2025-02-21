#pragma once

#include "MysqlConPool.h"

struct UserInfo {
  std::string name;
  std::string pass;
  std::string email;
  int uid;
};

class MysqlDao {
public:
  MysqlDao();
  ~MysqlDao();
  int RegUser(const std::string &name, const std::string &email,
              const std::string &pass);

  bool CheckEmail(const std::string &name, const std::string &email);
  bool UpdatePasswd(const std::string &name, const std::string &pass);
  bool CheckPasswd(const std::string &name, const std::string &pass,
                   UserInfo &userinfo);

private:
  std::unique_ptr<MysqlConPool> pool_;
};
