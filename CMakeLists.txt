cmake_minimum_required(VERSION 3.5)

project(erd_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -g3)
set(CMAKE_CXX_FLAGS -O0)
set(CMAKE_CXX_FLAGS -pedantic-errors)
set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS -Werror)

add_executable(erd_lib main.cpp)
