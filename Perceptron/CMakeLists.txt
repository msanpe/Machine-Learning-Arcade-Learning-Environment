cmake_minimum_required(VERSION 3.6)
project(Perceptron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    perceptron.cpp)

add_executable(Perceptron ${SOURCE_FILES})