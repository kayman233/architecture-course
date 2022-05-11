cmake_minimum_required(VERSION 3.21)
project(Banks)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(architecture_course
        src/Transaction.cpp
        src/Account.cpp
        src/main.cpp
        src/Bank.cpp
        src/Client.cpp)
