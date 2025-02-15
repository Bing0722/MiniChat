cmake_minimum_required(VERSION 3.30)

project(redis_test)

find_package(Boost REQUIRED filesystem system)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

add_executable(${PROJECT_NAME} main.cc ../../RedisMgr.cc ../../RedisConPool.cc ../../ConfigMgr.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE ${HIREDIS_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem Boost::system)
