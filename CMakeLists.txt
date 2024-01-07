cmake_minimum_required(VERSION 3.12)
project(engine)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_program
    src/main.cpp
)

target_include_directories(my_program PRIVATE
    include
)
