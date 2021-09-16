cmake_minimum_required(VERSION 3.15)
project(JPGtoSVG)

set(CMAKE_CXX_STANDARD 14)

include_directories(base64)

add_executable(JPGtoSVG
        base64/base64.cpp
        base64/base64.h
        convert.cpp)
