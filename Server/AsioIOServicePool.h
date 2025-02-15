#pragma once

#include "Singleton.h"
#include <boost/asio.hpp>
#include <vector>

class AsioIOServicePool : public Singleton<AsioIOServicePool> {
  friend Singleton<AsioIOServicePool>;

public:
  using IOService = boost::asio::io_context;
  // executor_work_guard 确保 io_context不会在没有活动任务的时提前停止
  using Work =
      boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
  using WorkPtr = std::unique_ptr<Work>;

  ~AsioIOServicePool();

  AsioIOServicePool(const AsioIOServicePool &) = delete;
  AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;

  // 使用round-robin方式返回一个 io_context
  boost::asio::io_context &GetIOService();

  void Stop();

private:
  AsioIOServicePool(
      std::size_t size = 2 /* std::thread::hardware_concurrency() */);

  std::vector<IOService> ioService_;
  std::vector<WorkPtr> works_;
  std::vector<std::thread> threads_;
  std::size_t netxIOService_;
};
