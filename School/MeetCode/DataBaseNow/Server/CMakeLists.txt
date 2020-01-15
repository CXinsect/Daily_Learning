cmake_minimum_required(VERSION 3.15)

#project(DataBase)

add_subdirectory(util)

add_executable(Demo redis.cc)

target_link_libraries(Demo status_lib)