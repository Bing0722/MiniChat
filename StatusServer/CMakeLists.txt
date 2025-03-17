cmake_minimum_required(VERSION 3.30)

project(StatusServer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(UNIX AND NOT APPLE)
  message(STATUS "使用 Linux 系统的配置")

  set(mysql-concpp_DIR "/home/bing/Downloads/mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit")
  set(MYSQL_CONCPP_LIBRARIES
    "/home/bing/Downloads/mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit/lib64/libmysqlcppconn.so"
    "/home/bing/Downloads/mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit/lib64/libmysqlcppconn.so.10"
    "/home/bing/Downloads/mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit/lib64/libmysqlcppconn.so.10.9.2.0")

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(JSONCPP REQUIRED jsoncpp)
  find_package(Boost REQUIRED filesystem system)
  find_package(Threads REQUIRED)
  pkg_check_modules(HIREDIS REQUIRED hiredis)
  find_package(mysql-concpp REQUIRED)

  # 设置 Protobuf 模块为兼容模式
  set(protobuf_MODULE_COMPATIBLE TRUE)
  find_package(Protobuf CONFIG REQUIRED)
  # 输出 Protobuf 的版本信息到 CMake 的构建日志
  message(STATUS "Using protobuf ${Protobuf_VERSION}")

  # 设置变量 _PROTOBUF_LIBPROTOBUF 为 Protobuf 库的 CMake 目标 libprotobuf
  set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
  # 设置变量 _REFLECTION 为 gRPC 库的反射模块 grpc++_reflection
  set(_REFLECTION gRPC::grpc++_reflection)

  find_package(gRPC CONFIG REQUIRED)
  # 输出 gRPC 的版本信息到 CMake 的构建日志
  message(STATUS "Using gRPC ${gRPC_VERSION}")

  # 设置变量 _GRPC_GRPCPP 为 gRPC 库的 CMake 目标 grpc++
  set(_GRPC_GRPCPP gRPC::grpc++)
elseif(WIN32)
  message(STATUS "使用 Windows 系统的配置")

  set(CMAKE_UTF8_EXTERNAL "1")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

  if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(-D_WIN32_WINNT=0x0601)
  endif()

  set(CMAKE_PREFIX_PATH "C:/Tools/mysql-connector-c++-9.2.0-winx64" CACHE STRING "Path to MySQL Connector C++ CMake files")

  include_directories("C:/Tools/mysql-connector-c++-9.2.0-winx64/include")
  link_directories("C:/Tools/mysql-connector-c++-9.2.0-winx64/lib64/vs14")

  set(MYSQL_CONCPP_ROOT "C:/Tools/mysql-connector-c++-9.2.0-winx64")
  set(MYSQL_CONCPP_LIBRARIES
        ${MYSQL_CONCPP_ROOT}/lib64/debug/vs14/mysqlcppconn.lib
        ${MYSQL_CONCPP_ROOT}/lib64/debug/vs14/mysqlcppconnx.lib
    )

  find_package(Boost REQUIRED filesystem system)
  find_package(jsoncpp CONFIG REQUIRED)
  find_package(gRPC REQUIRED)
  find_package(Protobuf REQUIRED)
  find_package(hiredis REQUIRED)
  # find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
  # find_package(mysql-concpp REQUIRED)
endif()

# 使用 file(GLOB ...) 命令将项目目录下所有 .cc 文件（源文件）加入到 SOURCE_FILES 变量中
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cc)


if(UNIX AND NOT APPLE)
  # 添加附加文件到 SOURCE_FILES
  list(APPEND SOURCE_FILES ./LINUX/message.pb.cc ./LINUX/message.grpc.pb.cc)
elseif(WIN32)
  # 添加附加文件到 SOURCE_FILES
  list(APPEND SOURCE_FILES ./WIN32/message.pb.cc ./WIN32/message.grpc.pb.cc)
endif()

# 创建一个名为 Server 的可执行文件，源文件来自于 SOURCE_FILES 变量
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


if(UNIX AND NOT APPLE)
  ######## Linux #########
  target_link_libraries(${PROJECT_NAME} PRIVATE
        Boost::filesystem
        Boost::system
        ${JSONCPP_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        ${MYSQL_CONCPP_LIBRARIES}
        # 将 gRPC 反射模块、gRPC 主库和 Protobuf 库链接到 Server 可执行文件中
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
    )

  target_include_directories(${PROJECT_NAME} PRIVATE ${JSONCPP_INCLUDE_DIRS})
elseif(WIN32)
  ######## Windows #########
  target_link_libraries(${PROJECT_NAME} PRIVATE
        ${Boost_LIBRARIES}
        jsoncpp_lib
        gRPC::grpc++
        protobuf::libprotobuf
        hiredis::hiredis
        ${MYSQL_CONCPP_LIBRARIES}
    )
endif()
