cmake_minimum_required(VERSION 3.10)
project(Lists)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O3 -lm -lc -pthread")

set(sources
    Distribution.cpp
    Types.cpp
    List.cpp 
    Tester.cpp)

add_executable(Tester ${sources})

add_executable(DistTest Distribution.cpp Types.cpp Distribution_test.cpp)