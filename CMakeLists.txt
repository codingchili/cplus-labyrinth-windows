cmake_minimum_required(VERSION 3.5)
project(assignment_windows)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Game.cpp Game.h)
add_executable(assignment_windows ${SOURCE_FILES})