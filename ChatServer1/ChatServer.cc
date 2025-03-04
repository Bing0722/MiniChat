#include "AsioIOServicePool.h"
#include "CServer.h"
#include "ConfigMgr.h"

bool bstop = false;
std::condition_variable cv;
std::mutex mtx;

int main() {
  try {
    // 读取配置文件
    auto &config = ConfigMgr::Instance();
    auto port = config["ChatServer"]["Port"];

    auto pool = AsioIOServicePool::GetInstance();

    boost::asio::io_context ioc;

    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc, pool](auto, auto) {
      ioc.stop();
      pool->Stop();
    });

    CServer s(ioc, atoi(port.c_str()));

    ioc.run();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
