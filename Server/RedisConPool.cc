#include "RedisConPool.h"

RedisConPool::RedisConPool(size_t poolSize, const char *host, int port,
                           const char *pass)
    : poolSize_(poolSize), host_(host), port_(port), stop_(false) {

  for (size_t i = 0; i < poolSize; i++) {
    // 建立连接
    auto *context = redisConnect(host, port);
    if (context == nullptr || context->err != 0) {
      if (context != nullptr) {
        redisFree(context);
      }
      continue;
    }

    // 进行认证
    auto reply = (redisReply *)redisCommand(context, "AUTH %s", pass);
    if (reply->type == REDIS_REPLY_ERROR) {
      std::cout << "认证失败" << std::endl;
      freeReplyObject(reply);
      continue;
    }

    freeReplyObject(reply);
    std::cout << "认证成功" << std::endl;
    connections_.push(context);
  }
}

RedisConPool::~RedisConPool() {
  std::lock_guard<std::mutex> lock(mutex_);
  while (!connections_.empty()) {
    auto context = connections_.front();
    connections_.pop();
    redisFree(context);
  }
}

redisContext *RedisConPool::GetConncetion() {
  std::unique_lock<std::mutex> lock(mutex_);
  cond_.wait(lock, [this]() {
    if (stop_) {
      return true;
    }
    return !connections_.empty();
  });

  if (stop_) {
    return nullptr;
  }

  auto *context = connections_.front();
  connections_.pop();
  return context;
}

void RedisConPool::ReturnConnection(redisContext *context) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (stop_) {
    return;
  }
  connections_.push(context);
  cond_.notify_one();
}

void RedisConPool::Close() {
  stop_ = true;
  cond_.notify_all();
}
