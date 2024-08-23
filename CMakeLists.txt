cmake_minimum_required(VERSION 3.15)
project(cpp-matrix)

set(CMAKE_CXX_STANDARD 23)

find_package(Catch2 REQUIRED)

include(test/tests.cmake)



