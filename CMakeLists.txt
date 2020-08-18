cmake_minimum_required(VERSION 3.16)
project(CompileTimeKatas)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/fizzbuzz)
include_directories(include/util)

add_executable(CompileTimeKatas
        include/fizzbuzz/fizzbuzz.hpp
        include/util/print_tuples.hpp
        include/util/strlit.hpp
        src/main.cpp)
