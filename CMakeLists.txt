cmake_minimum_required (VERSION 2.6)
project(NucMath)

#include_directories(./src/)

file(GLOB SOURCES "src/*.cpp")

add_executable(NucMath ${SOURCES})
