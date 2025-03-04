#include "MysqlConPool.h"

MysqlConPool::MysqlConPool(const std::string &url, const std::string &user,
                           const std::string &pass, const std::string &schema,
                           int poolsize)
    : url_(url), user_(user), pass_(pass), schema_(schema), poolSize_(poolsize),
      stop_(false) {
  try {
    for (int i = 0; i < poolsize; ++i) {
      // 创建连接
      sql::mysql::MySQL_Driver *driver =
          sql::mysql::get_mysql_driver_instance();
      std::unique_ptr<sql::Connection> con(driver->connect(url_, user_, pass_));

      // 选择数据库
      con->setSchema(schema_);
      pool_.push(std::move(con));
    }
  } catch (sql::SQLException &e) {
    // 处理异常
    std::cerr << "mysql pool init failed!" << e.what() << '\n';
  }
}

std::unique_ptr<sql::Connection> MysqlConPool::GetConnection() {
  std::unique_lock<std::mutex> lock(mtx_);
  cv_.wait(lock, [this] { return !pool_.empty() || stop_; });
  if (stop_) {
    return nullptr;
  }

  std::unique_ptr<sql::Connection> con(std::move(pool_.front()));
  pool_.pop();
  return con;
}

void MysqlConPool::ReturnConnection(std::unique_ptr<sql::Connection> con) {
  std::unique_lock<std::mutex> lock(mtx_);
  if (stop_) {
    return;
  }
  pool_.push(std::move(con));
  cv_.notify_one();
}

void MysqlConPool::Close() {
  stop_ = true;
  cv_.notify_all();
}

MysqlConPool::~MysqlConPool() {
  std::unique_lock<std::mutex> lock(mtx_);
  while (!pool_.empty()) {
    pool_.pop();
  }
}
