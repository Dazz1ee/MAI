#include <string>
#include <vector>
using namespace std;

#ifndef KP__NODE_H_
#define KP__NODE_H_

class Node {
 public:
  Node() {}

  bool operator ==(Node other_node) const{
    return id_ == other_node.getId();
  }

  Node(int id, string type, vector<int> child_id, string mutex_name, vector<string> files) {
    id_ = id;
    type_ = type;
    child_id_ = child_id;
    mutex_name_ = mutex_name;
    files_ = files;
  }

  int getId(){
    return id_;
  }

  vector<int> getChildrenId(){
    return child_id_;
  }
  string getType(){
    return type_;
  }
  string getMutex(){
    return mutex_name_;
  }

  void setMutex(string new_mutex_name){
    mutex_name_ = new_mutex_name;
  }

  vector<string> getFiles(){
    return files_;
  }
 private:
  int id_;
  string type_;
  vector<int> child_id_;
  string mutex_name_;
  vector<string> files_;
};

#endif //KP__NODE_H_
