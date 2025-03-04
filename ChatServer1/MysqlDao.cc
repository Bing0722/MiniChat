#include "MysqlDao.h"
#include "ConfigMgr.h"

MysqlDao::MysqlDao() {
  auto &config = ConfigMgr::Instance();
  const auto &host = config["Mysql"]["Host"];
  const auto &port = config["Mysql"]["Port"];
  const auto &pass = config["Mysql"]["Pass"];
  const auto &schema = config["Mysql"]["Schema"];
  const auto &user = config["Mysql"]["User"];
  pool_.reset(new MysqlConPool(host + ":" + port, user, pass, schema, 5));
}

MysqlDao::~MysqlDao() { pool_->Close(); }

int MysqlDao::RegUser(const std::string &name, const std::string &email,
                      const std::string &pass) {
  auto con = pool_->GetConnection();
  if (con == nullptr) {
    pool_->ReturnConnection(std::move(con));
    return false;
  }
  try {
    // 插入数据
    std::unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("CALL reg_user(?,?,?,@result)"));
    // 设置参数
    stmt->setString(1, name);
    stmt->setString(2, email);
    stmt->setString(3, pass);

    stmt->execute();

    // 查询数据
    std::unique_ptr<sql::Statement> stmtResult(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmtResult->executeQuery("SELECT @result AS result"));

    if (res->next()) {
      int result = res->getInt("result");
      std::cout << "Result: " << result << std::endl;
      pool_->ReturnConnection(std::move(con));
      return result;
    }
    pool_->ReturnConnection(std::move(con));
    return -1;

  } catch (sql::SQLException &e) {
    pool_->ReturnConnection(std::move(con));
    std::cerr << "SQLException: " << e.what();
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return -1;
  }
}

bool MysqlDao::CheckEmail(const std::string &name, const std::string &email) {
  auto con = pool_->GetConnection();
  if (con == nullptr) {
    pool_->ReturnConnection(std::move(con));
    return false;
  }
  try {
    // 准备查询语句
    std::unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("SELECT email FROM user WHERE name = ?"));
    // 设置参数
    stmt->setString(1, name);

    // 执行查询
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    // 遍历结果集
    while (res->next()) {
      std::cout << "Check Email: " << res->getString("email") << std::endl;
      if (email != res->getString("email")) {
        pool_->ReturnConnection(std::move(con));
        return false;
      }
    }
    pool_->ReturnConnection(std::move(con));
    return false;

  } catch (sql::SQLException &e) {
    pool_->ReturnConnection(std::move(con));
    std::cerr << "SQLException: " << e.what();
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return false;
  }
}

bool MysqlDao::UpdatePasswd(const std::string &name, const std::string &pass) {
  auto con = pool_->GetConnection();
  if (con == nullptr) {
    return false;
  }
  try {
    // 准备查询语句
    std::unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("UPDATE user SET pass = ? WHERE name = ?"));
    // 设置参数
    stmt->setString(2, name);
    stmt->setString(1, pass);

    // 执行更新
    int updateCount = stmt->executeUpdate();

    std::cout << "Update rows: " << updateCount << std::endl;

    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    pool_->ReturnConnection(std::move(con));
    return true;

  } catch (sql::SQLException &e) {
    pool_->ReturnConnection(std::move(con));
    std::cerr << "SQLException: " << e.what();
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return false;
  }
}

bool MysqlDao::CheckPasswd(const std::string &name, const std::string &pass,
                           UserInfo &userinfo) {
  auto con = pool_->GetConnection();
  // TODO Defer 类要写 ?
  if (con == nullptr) {
    pool_->ReturnConnection(std::move(con));
    return false;
  }
  try {
    // 准备SQL语句
    std::unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("SELECT * FROM user WHERE name = ?"));
    // 设置参数
    stmt->setString(1, name);

    // 执行查询
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
    std::string origin_pass = "";

    // 遍历结果集
    while (res->next()) {
      origin_pass = res->getString("pass");
      // 输出查询的密码
      std::cout << "Passwd: " << origin_pass << std::endl;
      break;
    }
    userinfo.name = name;
    userinfo.email = res->getString("email");
    userinfo.uid = res->getInt("uid");
    userinfo.pass = origin_pass;
    pool_->ReturnConnection(std::move(con));
    return true;

  } catch (sql::SQLException &e) {
    pool_->ReturnConnection(std::move(con));
    std::cerr << "SQLException: " << e.what();
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return false;
  }
}

std::shared_ptr<UserInfo> MysqlDao::GetUser(int uid) {
  auto con = pool_->GetConnection();
  if (con == nullptr) {
    pool_->ReturnConnection(std::move(con));
    return nullptr;
  }
  try {
    // 准备SQL语句
    std::unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("SELECT * FROM user WHERE uid = ?"));
    stmt->setInt(1, uid);

    // 执行查询
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
    std::shared_ptr<UserInfo> user_ptr = nullptr;
    // 遍历结果集
    while (res->next()) {
      user_ptr.reset(new UserInfo);
      user_ptr->pass = res->getString("pass");
      user_ptr->email = res->getString("email");
      user_ptr->nick = res->getString("nick");
      user_ptr->desc = res->getString("desc");
      user_ptr->sex = res->getInt("sex");
      user_ptr->icon = res->getString("icon");
      user_ptr->uid = uid;
      break;
    }

    pool_->ReturnConnection(std::move(con));
    return user_ptr;
  } catch (sql::SQLException &e) {
    pool_->ReturnConnection(std::move(con));
    std::cerr << "SQLException: " << e.what() << std::endl;
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return nullptr;
  }
}

std::shared_ptr<UserInfo> MysqlDao::GetUser(std::string name) {
  auto con = pool_->GetConnection();
  if (con == nullptr) {
    pool_->ReturnConnection(std::move(con));
    return nullptr;
  }
  try {
    // 准备SQL语句
    std::unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("SELECT * FROM user WHERE name = ?"));
    stmt->setString(1, name);

    // 执行查询
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
    std::shared_ptr<UserInfo> user_ptr = nullptr;
    // 遍历结果集
    while (res->next()) {
      user_ptr.reset(new UserInfo);
      user_ptr->pass = res->getString("pass");
      user_ptr->email = res->getString("email");
      user_ptr->nick = res->getString("nick");
      user_ptr->desc = res->getString("desc");
      user_ptr->sex = res->getInt("sex");
      user_ptr->uid = res->getInt("uid");
      break;
    }

    pool_->ReturnConnection(std::move(con));
    return user_ptr;

  } catch (sql::SQLException &e) {
    pool_->ReturnConnection(std::move(con));
    std::cerr << "SQLException: " << e.what() << std::endl;
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return nullptr;
  }
}