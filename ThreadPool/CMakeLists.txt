cmake_minimum_required(VERSION 3.27)
project(CLion)

set(CMAKE_CXX_STANDARD 20)
add_executable(CLion
        TaskQueue.cpp
        TaskQueue.h
        ThreadPool.cpp
        ThreadPool.h
        main.cpp
        TaskQueue.cpp
)
