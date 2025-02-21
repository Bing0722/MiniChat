#include "CServer.h"
#include "ConfigMgr.h"

int main() {
  // 读取配置文件
  auto &config = ConfigMgr::Instance();
  std::string gate_port_str = config["GateServer"]["Port"];
  unsigned short gate_port = atoi(gate_port_str.c_str());

  try {
    unsigned short port = static_cast<unsigned short>(gate_port);
    // 提供一个线程用于处理IO操作
    net::io_context ioc{1};
    // 创建一个 signal_set对象用于处理特定的信号
    // SIGINT 通常是用户按下 Ctrl+C
    // SIGTERM 是一个终止信号
    boost::asio::signal_set singals(ioc, SIGINT, SIGTERM);
    // 异步处理信号
    singals.async_wait(
        [&ioc](const boost::system::error_code &ec, int signal_number) {
          if (ec) {
            return;
          }
          ioc.stop();
        });
    // 创建一个 CServer类的共享指针
    std::make_shared<CServer>(ioc, port)->start();
    // 进入时间循环 等待异步操作
    ioc.run();
  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
