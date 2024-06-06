cmake_minimum_required(VERSION 3.27)
project(windows_gaming_input_util)

set(CMAKE_CXX_STANDARD 17)

add_executable(windows_gaming_input_util main.cpp
        helper.cpp
        helper.h
)
