cmake_minimum_required(VERSION 3.10)
project(MinCount)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O3 -lm -lc -pthread")

set(sources 
    Tester.cpp
    MinCount.cpp
    Set.cpp
    )

add_executable(Tester ${sources})
add_executable(HS StatAnalyzer.cpp Set.cpp)
add_executable(Op OptimalK.cpp Set.cpp MinCount.cpp)
add_executable(BH BadHashTester.cpp Set.cpp MinCount.cpp ConcreateHashFunction.hpp)