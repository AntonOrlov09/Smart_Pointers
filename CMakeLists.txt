cmake_minimum_required(VERSION 3.15)
project(smart_pointers)

set(CMAKE_CXX_STANDARD 17)

add_executable(smart_pointers main.cpp scoped_ptr.hpp "shared_ptr .hpp")