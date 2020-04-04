cmake_minimum_required(VERSION 3.15)
project(mandrake)

set(CMAKE_CXX_STANDARD 20)

add_executable(mandrake main.cpp http.cpp http.h)