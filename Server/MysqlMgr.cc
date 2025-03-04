#include "MysqlMgr.h"

MysqlMgr::MysqlMgr() {}

MysqlMgr::~MysqlMgr() {
  std::cout << "The Mysql server is being destroyed..." << std::endl;
}

int MysqlMgr::RegUser(const std::string &name, const std::string &email,
                      const std::string &pass) {
  return dao_.RegUser(name, email, pass);
}

bool MysqlMgr::CheckEmail(const std::string &name, const std::string &email) {
  return dao_.CheckEmail(name, email);
}

bool MysqlMgr::UpdatePasswd(const std::string &name, const std::string &pass) {
  return dao_.UpdatePasswd(name, pass);
}

bool MysqlMgr::CheckPasswd(const std::string &name, const std::string &pass,
                           UserInfo &userinfo) {
  return dao_.CheckPasswd(name, pass, userinfo);
}