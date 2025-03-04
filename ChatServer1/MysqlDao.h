#pragma once

#include "MysqlConPool.h"
#include "data.h"

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

  std::shared_ptr<UserInfo> GetUser(int uid);
  std::shared_ptr<UserInfo> GetUser(std::string name);

private:
  std::unique_ptr<MysqlConPool> pool_;
};
