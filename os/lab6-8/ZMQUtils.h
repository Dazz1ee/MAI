#include "zmq.hpp"
#include <string>

#ifndef LAB6_8__ZMQUTILS_H_
#define LAB6_8__ZMQUTILS_H_

std::string createNameSocket(int node_id);

std::string createNameSocketToChildren(int node_id);

std::string createNameSocketToParent(int node_id);

#endif