#include <iostream>
#include <string>
#include "zmq.hpp"
#include "message.h"
#include "ZMQUtils.h"


static zmq::context_t context;
static zmq::socket_t pub_sock_to_children(context, ZMQ_PUB);
static zmq::socket_t pub_sock_to_parent(context, ZMQ_PUB);
static zmq::socket_t sub_sock(context, ZMQ_SUB);

static std::string pub_socket_to_children_name;
static std::string pub_socket_to_server_name;

static std::string parent_socket_name;
static std::vector<std::string> children_sockets_name;

static int node_id;
static bool run;

void client_init(int argc, char* argv[]){
  node_id = atoi(argv[1]);

  parent_socket_name = argv[2];

  pub_socket_to_children_name = createNameSocketToChildren(node_id);
  pub_socket_to_server_name = createNameSocketToParent(node_id);

  pub_sock_to_children.bind(pub_socket_to_children_name);
  pub_sock_to_parent.bind(pub_socket_to_server_name);

  sub_sock.connect(parent_socket_name);
  sub_sock.setsockopt(ZMQ_SUBSCRIBE, 0, 0);

  run = true;
}

void exec(zmq::message_t& data) {
  ExecBody body = getMessageExec(data);
  std::vector<int> entries;

  std::string::size_type position = 0;
  while(std::string::npos != (position = body.text.find(body.pattern, position))){
	entries.push_back(position);
	++position;
  }

  zmq::message_t answer = fillMessageExecAnswer(entries);
  pub_sock_to_parent.send(answer);
}

void ping(zmq::message_t& data) {
  zmq::message_t answer = fillMessagePingAnswer(node_id);
  pub_sock_to_parent.send(answer);
}
void removeNode(Header* header) {
  pub_sock_to_parent.close();
  sub_sock.close();
  context.close();
  exit(0);
}


void task(zmq::message_t& data){
  Header* header = getMessageHeader(data);;
  if(header->type == messageExec)
	exec(data);
  else if(header->type == messagePing)
	ping(data);
  else if(header->type == messageRemove)
	removeNode(header);
}

void client_run(){
  while(run){
	  zmq::message_t data;
	  sub_sock.recv(data);
	  Header* header = getMessageHeader(data);
	  if(header->to_id_node == node_id)
		task(data);
	  if(header->to_id_node != node_id){
		if(header->direction == toServer)
		  pub_sock_to_parent.send(data);
		else
		  pub_sock_to_children.send(data);
	  }
  }
}


int main(int argc, char* argv[]) {
  client_init(argc, argv);
  client_run();
}

