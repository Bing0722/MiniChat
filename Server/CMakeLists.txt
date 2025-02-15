cmake_minimum_required(VERSION 3.30)

project(Server)

set(CMAKE_CXX_STANDARD 17)

# 查找并引入 Boost 库的 filesystem 和 system 模块
find_package(Boost REQUIRED filesystem system)
# 查找并引入 pkg-config 工具，它通常用于管理外部库的路径和版本信息
find_package(PkgConfig REQUIRED)
# 使用 pkg-config 查找并引入 jsoncpp 库
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
# 查找并引入线程库。此行通常用于支持多线程操作
find_package(Threads REQUIRED)

# 设置 Protobuf 模块为兼容模式
set(protobuf_MODULE_COMPATIBLE TRUE)
# 查找并引入 Protobuf 库
find_package(Protobuf CONFIG REQUIRED)
# 输出 Protobuf 的版本信息到 CMake 的构建日志
message(STATUS "Using protobuf ${Protobuf_VERSION}")

# 设置变量 _PROTOBUF_LIBPROTOBUF 为 Protobuf 库的 CMake 目标 libprotobuf
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
# 设置变量 _REFLECTION 为 gRPC 库的反射模块 grpc++_reflection
set(_REFLECTION gRPC::grpc++_reflection)

# 查找并引入 gRPC 库
find_package(gRPC CONFIG REQUIRED)
# 输出 gRPC 的版本信息到 CMake 的构建日志
message(STATUS "Using gRPC ${gRPC_VERSION}")

# 设置变量 _GRPC_GRPCPP 为 gRPC 库的 CMake 目标 grpc++
set(_GRPC_GRPCPP gRPC::grpc++)

# 使用 file(GLOB ...) 命令将项目目录下所有 .cc 文件（源文件）加入到 SOURCE_FILES 变量中
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cc)

# 添加附加文件到 SOURCE_FILES
list(APPEND SOURCE_FILES message.pb.cc message.grpc.pb.cc)

# 创建一个名为 Server 的可执行文件，源文件来自于 SOURCE_FILES 变量
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 将 Boost::filesystem 和 Boost::system 库链接到 Server 可执行文件中
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem Boost::system)

# 将 jsoncpp 库链接到 Server 可执行文件中
target_link_libraries(${PROJECT_NAME} PRIVATE ${JSONCPP_LIBRARIES})
# 将 jsoncpp 的头文件目录加入到 Server 的编译器搜索路径中
target_include_directories(${PROJECT_NAME} PRIVATE ${JSONCPP_INCLUDE_DIRS})

# 将 gRPC 反射模块、gRPC 主库和 Protobuf 库链接到 Server 可执行文件中
target_link_libraries(${PROJECT_NAME} PRIVATE ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})

