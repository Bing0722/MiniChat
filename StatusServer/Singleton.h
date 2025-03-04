#pragma once

#include <iostream>
#include <memory>
#include <mutex>

template <typename T> class Singleton {
protected:
  Singleton() = default;
  Singleton(const Singleton<T> &) = delete;
  Singleton &operator=(const Singleton<T> &) = delete;

  static std::shared_ptr<T> instance_;

public:
  static std::shared_ptr<T> GetInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() { instance_ = std::shared_ptr<T>(new T); });
    return instance_;
  }

  void printAddress() { std::cout << instance_.get() << std::endl; }

  ~Singleton() { std::cout << "this is singleton destruct" << std::endl; }
};

template <typename T> std::shared_ptr<T> Singleton<T>::instance_ = nullptr;
