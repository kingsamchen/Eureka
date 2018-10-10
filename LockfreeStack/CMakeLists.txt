cmake_minimum_required(VERSION 3.12)

project(LockfreeStack)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(LockfreeStack
               lstack.h
               main.cpp)

target_link_libraries(LockfreeStack PUBLIC atomic pthread)