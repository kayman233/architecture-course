cmake_minimum_required(VERSION 3.21)
project(architecture_course)

set(SOURCE_LIB account.cpp)
add_library(Account SHARED ${SOURCE_LIB})
