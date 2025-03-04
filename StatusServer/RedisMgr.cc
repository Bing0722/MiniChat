#include "RedisMgr.h"
#include "ConfigMgr.h"
#include "RedisConPool.h"

RedisMgr::RedisMgr() {
  auto &config = ConfigMgr::Instance();
  auto host = config["Redis"]["Host"];
  auto port = config["Redis"]["Port"];
  auto pass = config["Redis"]["Pass"];
  con_pool_.reset(
      new RedisConPool(5, host.c_str(), atoi(port.c_str()), pass.c_str()));
}

RedisMgr::~RedisMgr() { Close(); }

bool RedisMgr::Get(const std::string &key, std::string &value) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "GET %s", key.c_str());

  if (reply == NULL) {
    std::cout << "[GET " << key << "] failed" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  if (reply->type != REDIS_REPLY_STRING) {
    std::cout << "[GET " << key << "] failed" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  value = reply->str;
  freeReplyObject(reply);

  std::cout << "Succeed to execute command [GET " << key << "]" << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::Set(const std::string &key, const std::string &value) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "SET %s %s", key.c_str(),
                                          value.c_str());

  if (reply == NULL) {
    std::cout << "Execut command [SET " << key << " " << value << "] failure!"
              << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  if (!(reply->type != REDIS_REPLY_STATUS &&
        (strcmp(reply->str, "OK") == 0 || strcmp(reply->str, "ok") == 0))) {
    std::cout << "Execut command [SET " << key << " " << value << "] failure!"
              << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return true;
  }

  freeReplyObject(reply);
  std::cout << "Execut command [SET " << key << " " << value << "] success!"
            << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::LPush(const std::string &key, const std::string &value) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "LPUSH %s %s", key.c_str(),
                                          value.c_str());

  if (reply == NULL) {
    std::cout << "Execut command [LPUSH " << key << " " << value << "] failure!"
              << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
    std::cout << "Execut command [LPUSH " << key << " " << value << "] failure!"
              << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  freeReplyObject(reply);
  std::cout << "Execut command [LPUSH " << key << " " << value << "] success!"
            << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::LPop(const std::string &key, std::string &value) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "LPOP %s", key.c_str());

  if (reply->type == REDIS_REPLY_NIL || reply == nullptr) {
    std::cout << "Execut command [LPOP " << key << "] failure!" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  value = reply->str;
  freeReplyObject(reply);
  std::cout << "Execut command [LPOP " << key << "] success!" << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::RPush(const std::string &key, const std::string &value) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "RPUSH %s %s", key.c_str(),
                                          value.c_str());

  if (reply == NULL) {
    std::cout << "Execut command [RPUSH " << key << " " << value << "] failure!"
              << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
    std::cout << "Execut command [RPUSH " << key << " " << value << "] failure!"
              << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  freeReplyObject(reply);
  std::cout << "Execut command [LPUSH " << key << " " << value << "] success!"
            << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::RPop(const std::string &key, std::string &value) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "RPOP %s", key.c_str());

  if (reply->type == REDIS_REPLY_NIL || reply == nullptr) {
    std::cout << "Execut command [RPOP " << key << "] failure!" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  value = reply->str;
  freeReplyObject(reply);
  std::cout << "Execut command [RPOP " << key << "] success!" << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::HSet(const std::string &key, const std::string &hkey,
                    const std::string &value) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "HSET %s %s %s", key.c_str(),
                                          hkey.c_str(), value.c_str());

  if (reply->type != REDIS_REPLY_INTEGER || reply == nullptr) {
    std::cout << "Execut command [HSET " << key << " " << hkey << " " << value
              << "] failure!" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  freeReplyObject(reply);
  std::cout << "Execut command [HSET " << key << " " << hkey << " " << value
            << "] success!" << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::Hset(const char *key, const char *hkey, const char *hvalue,
                    size_t hvaluelen) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  const char *argv[4];
  size_t argvlen[4];
  argv[0] = "HSET";
  argvlen[0] = 4;
  argv[1] = key;
  argvlen[1] = strlen(key);
  argv[2] = hkey;
  argvlen[2] = strlen(hkey);
  argv[3] = hvalue;
  argvlen[3] = hvaluelen;

  auto reply = (redisReply *)redisCommandArgv(connect, 4, argv, argvlen);

  if (reply->type != REDIS_REPLY_INTEGER || reply == nullptr) {
    std::cout << "Execut command [HSET " << key << " " << hkey << " " << hvalue
              << "] failure!" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  freeReplyObject(reply);
  std::cout << "Execut command [HSET " << key << " " << hkey << " " << hvalue
            << "] success!" << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

std::string RedisMgr::HGet(const std::string &key, const std::string &hkey) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return "";
  }

  const char *argv[3];
  size_t argvlen[3];
  argv[0] = "HGET";
  argvlen[0] = 4;
  argv[1] = key.c_str();
  argvlen[1] = key.length();
  argv[2] = hkey.c_str();
  argvlen[2] = hkey.length();

  auto reply = (redisReply *)redisCommandArgv(connect, 3, argv, argvlen);

  if (reply->type == REDIS_REPLY_NIL || reply == nullptr) {
    std::cout << "Execut command [HGET " << key << " " << hkey << "] failure!"
              << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return "";
  }

  std::string value = reply->str;
  freeReplyObject(reply);
  std::cout << "Execut command [HGET " << key << " " << hkey << "] success!"
            << std::endl;
  con_pool_->ReturnConnection(connect);
  return value;
}

bool RedisMgr::Del(const std::string &key) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "DEL %s", key.c_str());

  if (reply->type != REDIS_REPLY_INTEGER || reply == nullptr) {
    std::cout << "Execut command [DEL " << key << "] failure!" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  freeReplyObject(reply);
  std::cout << "Execut command [DEL " << key << "] success!" << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

bool RedisMgr::ExistsKey(const std::string &key) {
  auto connect = con_pool_->GetConncetion();

  if (connect == nullptr) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(connect, "EXISTS %s", key.c_str());

  if (reply->type != REDIS_REPLY_INTEGER || reply == nullptr ||
      reply->integer == 0) {
    std::cout << "Not Found [KEY " << key << "]!" << std::endl;
    freeReplyObject(reply);
    con_pool_->ReturnConnection(connect);
    return false;
  }

  freeReplyObject(reply);
  std::cout << "Found [KEY " << key << "] EXISTS!" << std::endl;
  con_pool_->ReturnConnection(connect);
  return true;
}

void RedisMgr::Close() { con_pool_->Close(); }
