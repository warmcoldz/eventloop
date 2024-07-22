cmake_minimum_required(VERSION 3.22)

project(turboquic)
set(CMAKE_CXX_STANDARD 23)

add_library(eventloop
    event_loop_internal_controller.h
    event_loop.cpp
    event_loop.h
    timer_handler.h
    timer_internal_controller.h
    timer.cpp
    timer.h
)

target_include_directories(eventloop PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(examples)
