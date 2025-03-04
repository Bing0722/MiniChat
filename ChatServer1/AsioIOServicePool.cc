#include "AsioIOServicePool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size)
    : ioService_(size), works_(size), netxIOService_(0) {

  for (std::size_t i = 0; i < size; i++) {
    works_[i] = std::unique_ptr<Work>(new Work(ioService_[i].get_executor()));
  }

  for (std::size_t i = 0; i < size; i++) {
    // 使用 emplace_back 可以在容器内构造函数
    // 如果使用 push_back 则是先创建对象再拷贝/移动
    threads_.emplace_back([this, i]() { ioService_[i].run(); });
  }
}

AsioIOServicePool::~AsioIOServicePool() {
  std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context &AsioIOServicePool::GetIOService() {
  auto &service = ioService_[netxIOService_];
  if (netxIOService_ == ioService_.size()) {
    netxIOService_ = 0;
  }
  return service;
}

void AsioIOServicePool::Stop() {

  // 解除 io_context 的保护
  for (auto &work : works_) {
    work->reset();
  }

  // 让所有的 io_context 停止工作
  for (auto &io_service : ioService_) {
    io_service.stop();
  }

  // 等待线程完成
  for (auto &t : threads_) {
    t.join();
  }
}
