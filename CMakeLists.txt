cmake_minimum_required(VERSION 3.22)

project(turboquic)
set(CMAKE_CXX_STANDARD 23)

add_library(eventloop INTERFACE event_loop.h)
target_include_directories(eventloop INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(examples)
