cmake_minimum_required(VERSION 3.21)
project(compile_experiment)

set(CMAKE_CXX_STANDARD 14)

add_executable(compile_experiment main.cpp scanner.cpp scanner.h parse.cpp parse.h semantics.cpp semantic.h)
