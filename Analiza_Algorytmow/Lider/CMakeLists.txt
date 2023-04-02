cmake_minimum_required(VERSION 3.10)
project(LeaderElection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O3")

set(sources 
    Tester.cpp 
    LeaderElection.cpp)

add_executable(Tester ${sources})