cmake_minimum_required(VERSION 3.12)
project(engine)

set(CMAKE_CXX_STANDARD 14)

add_executable(run
    src/main.cpp
    src/board.cpp
    src/engine.cpp
    src/game.cpp
    src/bit_operator.cpp
    src/move_generator.cpp
)

target_include_directories(run PRIVATE
    include
)
