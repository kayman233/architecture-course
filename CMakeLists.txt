cmake_minimum_required(VERSION 3.21)
project(architecture_course)

set(CMAKE_CXX_STANDARD 14)

include_directories(includes)
add_subdirectory(includes)

add_executable(architecture_course
        src/main.cpp)

target_link_libraries(architecture_course Account)

