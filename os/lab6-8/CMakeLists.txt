cmake_minimum_required(VERSION 3.21)
project(lab6_8)


add_executable(client client.cpp  ZMQUtils.cpp message.cpp)
add_executable(server server.cpp  ZMQUtils.cpp message.cpp)

target_link_libraries(server zmq)
target_link_libraries(client zmq)
