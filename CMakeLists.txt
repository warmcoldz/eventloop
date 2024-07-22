cmake_minimum_required(VERSION 3.22)

project(velocizzio)
set(CMAKE_CXX_STANDARD 23)

add_library(velocizzio_lib
    include/velocizzio/event_loop.h
    include/velocizzio/timer_handler.h
    include/velocizzio/timer.h

    include/velocizzio/detail/event_loop_internal_controller.h
    include/velocizzio/detail/timer_internal_controller.h

    src/event_loop.cpp
    src/timer.cpp
)

target_include_directories(velocizzio_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/velocizzio)
target_include_directories(velocizzio_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(examples)
