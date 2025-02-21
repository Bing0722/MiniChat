cmake_minimum_required(VERSION 3.30)

project(Mysql)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_UTF8_EXTERNAL "1")

if(MSVC)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

set(CMAKE_TOOLCHAIN_FILE "D:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
include_directories("D:/Tools/vcpkg/installed/x64-windows/include")
link_directories("D:/Tools/vcpkg/installed/x64-windows/lib")

find_package(Boost REQUIRED filesystem system) 
find_package(jsoncpp CONFIG REQUIRED)
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(hiredis REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)

# 创建可执行文件
add_executable(${PROJECT_NAME} main.cc
)

# 链接 jsoncpp 库
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${Boost_LIBRARIES} 
    jsoncpp_lib
    gRPC::grpc++
    protobuf::libprotobuf
    hiredis::hiredis
    unofficial::mysql-connector-cpp::connector
)
