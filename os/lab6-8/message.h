#include "zmq.hpp"
#include <string>
#include <vector>

#ifndef LAB6_8__MESSAGE_H_
#define LAB6_8__MESSAGE_H_


enum message_type_t {
  messagePing,
  messageCreate,
  messageRemove,
  messageExec,
  messageExecAnswer,
  messagePingAnswer,
  messageCreateAnswer,
};

enum MessageDirection{
  toClient,
  toServer,
};

struct Header {
  MessageDirection direction;
  message_type_t type;
  int to_id_node;
};

struct CreateBody {
  int child_id;
};

struct ExecBody {
  std::string text;
  std::string pattern;
};

struct pingBodyAnswer {
  int node_id;
};

struct exec_body_answer {
  std::vector<int> entries;
};

struct create_body_answer {
  int proccess_id;
  std::string error;
};
create_body_answer getMessageCreateAnswer(zmq::message_t& data);
zmq::message_t fillMessageExec(int endPoint, std::string& text, std::string& pattern);

zmq::message_t fillMessageExecAnswer(std::vector<int>& enrties);

zmq::message_t fillMessageCreate(int parent_id, int child_id);

zmq::message_t fillMessageCreateAnswer(int pid, std::string error);
zmq::message_t fillMessageRemove(int endPoint);
zmq::message_t fillMessagePing(int endPoint);

zmq::message_t fillMessagePingAnswer(int src_id);

Header* getMessageHeader(zmq::message_t& data);

CreateBody getMessageCreate(zmq::message_t& data);

ExecBody getMessageExec(zmq::message_t& data);

exec_body_answer getMessageExecAnswer(zmq::message_t& data);

pingBodyAnswer getMessagePingAnswer(zmq::message_t& data);

#endif //LAB6_8__MESSAGE_H_
