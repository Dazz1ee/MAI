#include "ZMQUtils.h"

std::string createNameSocket(int node_id){
  std::string name = "ipc://sock" + std::to_string(node_id+5000);
  return name;
}

std::string createNameSocketToChildren(int node_id){
  std::string name = createNameSocket(node_id) +"_to_children";
  return name;
}

std::string createNameSocketToParent(int node_id){
  std::string name = createNameSocket(node_id) +"_to_parent";
  return name;
}