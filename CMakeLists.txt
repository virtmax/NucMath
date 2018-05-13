cmake_minimum_required (VERSION 2.6)
project(NucMath)

include_directories(./src/)
#include_directories(./tests/)

file(GLOB SOURCES "./src/*.cpp" "./tests/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O0 --coverage") # debug, no optimisation, enabling coverage

add_executable(NucMath ${SOURCES})
