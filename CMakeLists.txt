cmake_minimum_required(VERSION 3.21)
project(Banks)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(architecture_course
        src/Transaction.cpp
        src/TransactionStorage.cpp
        src/Account.cpp
        src/main.cpp
        src/Bank.cpp
        src/Client.cpp)
