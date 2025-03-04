#pragma once

#include <atomic>
#include <condition_variable>
#include <hiredis/hiredis.h>
#include <hiredis/read.h>
#include <iostream>
#include <mutex>
#include <queue>

// redis 连接池
class RedisConPool {
public:
  RedisConPool(size_t poolSize, const char *host, int port, const char *pass);

  ~RedisConPool();

  redisContext *GetConncetion();

  void ReturnConnection(redisContext *context);

  void Close();

private:
  std::atomic<bool> stop_;
  size_t poolSize_;
  const char *host_;
  int port_;
  std::queue<redisContext *> connections_;
  std::mutex mutex_;
  std::condition_variable cond_;
};
