cmake_minimum_required(VERSION 3.30)

project(Server)

find_package(Boost REQUIRED filesystem system)

find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP REQUIRED jsoncpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cc)

add_executable(Server ${SOURCE_FILES})

target_link_libraries(Server PRIVATE Boost::filesystem Boost::system)

target_link_libraries(Server PRIVATE ${JSONCPP_LIBRARIES})
target_include_directories(Server PRIVATE ${JSONCPP_INCLUDE_DIRS})

