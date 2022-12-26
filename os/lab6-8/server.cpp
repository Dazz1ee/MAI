#include <iostream>
#include <string>
#include "zmq.hpp"
#include <unistd.h>
#include "message.h"
#include "ZMQUtils.h"
#include "Tree.h"


static zmq::context_t context;

static zmq::socket_t publish_socket(context, ZMQ_PUB);
static zmq::socket_t sub_sock(context, ZMQ_SUB);

static std::string socket_name;
static std::vector<std::string> sockets_of_children;

Node* tree = init_tree(0);

void server_init(){

	socket_name = createNameSocket(0);
	publish_socket.bind(socket_name);

	sub_sock.setsockopt(ZMQ_SUBSCRIBE, nullptr, 0);
	int timeout = 300;

	sub_sock.setsockopt(ZMQ_RCVTIMEO, timeout);

}


bool receive_msg(zmq::message_t& msg){
  zmq::recv_result_t res;
  res = sub_sock.recv(msg);

  return res.has_value();
}

bool is_exists(int id) {
  zmq::message_t data = fillMessagePing(id);
  publish_socket.send(data, 0);

  zmq::message_t receive_data;
  Header* header;

  if(!receive_msg(receive_data))
	return false;

  pingBodyAnswer ans = getMessagePingAnswer(receive_data);
  if(ans.node_id != id)
	return false;

  return true;
}

void create(){
  int new_node_pid = -1;
  int child_id;
  std::cin >> child_id;

  if(is_exists(child_id)){
	std::cout << "Error: Already exists" << std::endl;
	return;
  }

  int fork_pid = fork();
  if(fork_pid == -1) {
	std::cout << "Error:fork: " << strerror(errno) << std::endl;
	return;
  }

  if(fork_pid == 0) {
	execl("client", "server", to_string(child_id).c_str(), socket_name.c_str(), NULL);
  }

  std::string parent_pub_socket_name = createNameSocketToParent(child_id);
  sockets_of_children.push_back(parent_pub_socket_name);
  sub_sock.connect(parent_pub_socket_name);

  new_node_pid = fork_pid;

  insert(tree, child_id);
std::cout << "OK: " << new_node_pid << std::endl;
}

void exec() {
  std::string text, pattern;
  int node_id;

  std::cin >> node_id;
  std::cin >> text;
  std::cin >> pattern;

  if(!is_exists(node_id)) {
	std::cout << "Error:" << node_id << ": Not found" << std::endl;
	return;
  }

  zmq::message_t data = fillMessageExec(node_id, text, pattern);
  publish_socket.send(data);

  zmq::message_t receive_data;
  Header* header;
  receive_msg(receive_data);
  exec_body_answer answer = getMessageExecAnswer(receive_data);

  std::cout << "OK:" << node_id << ": [";
  if(answer.entries.size() > 0)
	std::cout << answer.entries[0];
  for(int i = 1; i < answer.entries.size(); ++i) {
	std::cout << "; " << answer.entries[i];
  }
  std::cout << "]" << std::endl;
}

void ping() {
  int dest_id;
  std::cin >> dest_id;
  if(is_exists(dest_id))
	std::cout << "OK: 1" << std::endl;
  else
	std::cout << "Error: Not found" << std::endl;
}

void removeNode(){
  int node_id;
  cin >> node_id;
  if (!find(tree, node_id)) {
	cout << "Error: Not found" << endl;
	return;
  }
  if(!is_exists(node_id)){
	cout << "Error: Node is unavailable" << endl;
	return;
  }

  for(int node : getChildren(tree, node_id)){
	zmq::message_t data = fillMessageRemove(node);
	publish_socket.send(data);
  }
  free_tree(search(tree, node_id));
  cout << "OK";
}

void server_run(){
  std::string input_cmd;
  while(std::cin >> input_cmd) {
	  if(input_cmd == "create")
		create();
	  else if(input_cmd == "exec")
		exec();
	  else if(input_cmd == "ping")
		ping();
	  else if(input_cmd == "remove")
		removeNode();
	  else {
		std::cout << "Not correct" << std::endl;
	  }

  }
}


int main() {
  server_init();
  server_run();
  sub_sock.close();
  publish_socket.close();
  context.close();
}
