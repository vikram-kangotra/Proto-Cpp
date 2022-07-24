cmake_minimum_required(VERSION 3.10)

project(Proto-cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(proto-cpp src/lexer.cpp src/main.cpp src/parser.cpp)
