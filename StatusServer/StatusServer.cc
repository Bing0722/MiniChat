#include "AsioIOServicePool.h"
#include "ConfigMgr.h"
#include "MysqlMgr.h"
#include "RedisMgr.h"
#include "StatusServiceImpl.h"
#include "global.h"

void RunServer() {
  // 读取配置文件
  auto &config = ConfigMgr::Instance();
  // StatusServer 服务地址
  std::string server_address(config["StatusServer"]["Host"] + ":" +
                             config["StatusServer"]["Port"]);

  StatusServiceImpl service;

  // 创建构建器
  grpc::ServerBuilder builder;
  // 监听端口和添加服务
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  // 构建并启动 gRPC 服务器
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // 创建 Boost.Asio 的 io_context
  boost::asio::io_context ioc;
  // 创建 signal_set 用于捕获 SIGNIT
  boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

  // 设置异步等待SIGINT信号
  signals.async_wait(
      [&server](const boost::system::error_code &error, int signal_number) {
        if (!error) {
          std::cout << "Shutting down server..." << std::endl;
          server->Shutdown(); // 优雅地关闭服务器
        }
      });

  // 在单独的线程中运行 io_context
  std::thread([&ioc]() { ioc.run(); }).detach();

  // 等待服务器关闭
  server->Wait();
  ioc.stop(); // 停止 io_context
}

int main() {
  try {
    RunServer();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
