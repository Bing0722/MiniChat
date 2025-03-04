#pragma once

#include "Singleton.h"
#include <hiredis/hiredis.h>
#include <string>

class RedisConPool;

class RedisMgr : public Singleton<RedisMgr>,
                 public std::enable_shared_from_this<RedisMgr> {
  friend class Singleton<RedisMgr>;

public:
  ~RedisMgr();
  // bool Connect(const std::string &host, int port);
  // bool Auth(const std::string &pass);
  bool Get(const std::string &key, std::string &value);
  bool Set(const std::string &key, const std::string &value);
  bool LPush(const std::string &key, const std::string &value);
  bool LPop(const std::string &key, std::string &value);
  bool RPush(const std::string &key, const std::string &value);
  bool RPop(const std::string &key, std::string &value);
  bool HSet(const std::string &key, const std::string &hkey,
            const std::string &value);
  bool Hset(const char *key, const char *hkey, const char *hvalue,
            size_t hvaluelen);
  std::string HGet(const std::string &key, const std::string &hkey);
  bool Del(const std::string &key);
  bool ExistsKey(const std::string &key);
  void Close();

private:
  RedisMgr();

  // redisContext *connect_;
  // redisReply *reply_;

  std::unique_ptr<RedisConPool> con_pool_;
};
