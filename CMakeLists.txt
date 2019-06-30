# cmake_minimum_required(VERSION <specify CMake version here>)
project(EzLang)

set(CMAKE_CXX_STANDARD 14)

add_executable(EzLang main.cpp utils.h ast.h)