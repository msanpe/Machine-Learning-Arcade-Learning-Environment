cmake_minimum_required(VERSION 3.6)
project(Neural_net)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        functions.cpp
        functions.h
        main.cpp
        data.cpp
        data.h
        nn.cpp
        nn.h)

add_executable(Neural_net ${SOURCE_FILES})