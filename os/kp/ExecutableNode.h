#include "Node.h"

#ifndef KP__EXECUTABLENODE_H_
#define KP__EXECUTABLENODE_H_

struct test {
  int qu;
  int value;
  bool operator>(const test& y) const { return qu > y.qu;}
  bool operator<(const test& y) const { return qu < y.qu; }
  bool operator==(const test& y) const { return value == y.value; }
  bool operator!=(const test& y) const { return value != y.value; }
};

struct ExecutableNode {
  Node node;
  int priority;
  int process_id;
  bool operator==(const ExecutableNode& other_node) const {
    return node == other_node.node;
  }
  bool operator>(const ExecutableNode& other_node) const {
    return priority > other_node.priority;
  }
};


#endif //KP__EXECUTABLENODE_H_
