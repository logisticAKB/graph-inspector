cmake_minimum_required(VERSION 3.14)
project(graph-inspector)

set(CMAKE_CXX_STANDARD 17)

add_executable(graph-inspector main.cpp graph.h graph.cpp)