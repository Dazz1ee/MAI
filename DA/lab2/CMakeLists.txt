cmake_minimum_required(VERSION 3.24)
project(tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(tree main.cpp AVL.cpp RequestHandler.cpp Vector.cpp LinkedList.cpp all.cpp)
