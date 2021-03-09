cmake_minimum_required(VERSION 3.16)
project(HW2 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-Wpedantic -Wall -Wextra -Werror")
include_directories(${PROJECT_SOURCE_DIR})

add_executable(HW2 main.c doubll2d.c)