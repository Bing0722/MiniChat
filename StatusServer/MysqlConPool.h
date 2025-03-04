#pragma once

#include "global.h"
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>

class MysqlConPool {
public:
  MysqlConPool(const std::string &url, const std::string &user,
               const std::string &pass, const std::string &schema,
               int poolsize);

  std::unique_ptr<sql::Connection> GetConnection();

  void ReturnConnection(std::unique_ptr<sql::Connection> con);

  void Close();

  ~MysqlConPool();

private:
  std::string url_;
  std::string user_;
  std::string pass_;
  std::string schema_;
  int poolSize_;
  std::queue<std::unique_ptr<sql::Connection>> pool_;
  std::mutex mtx_;
  std::condition_variable cv_;
  std::atomic<bool> stop_;
};