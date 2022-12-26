#include "message.h"
#include <cstring>
zmq::message_t fillMessageExec(int endPoint, std::string& text, std::string& pattern){
  Header header {
	  toClient,
	  messageExec,
	  endPoint,
  };

  int size_of_message = sizeof(Header) + text.size() + pattern.size() + 2 * sizeof(int);
  zmq::message_t data(size_of_message);
  char* memory_pointer = (char*)data.data();

  std::memcpy(memory_pointer, &header, sizeof(header));
  memory_pointer += sizeof(header);

  int text_size = text.size();
  std::memcpy(memory_pointer, &text_size, sizeof(text_size));
  memory_pointer += sizeof(text_size);

  std::copy(text.begin(), text.end(), memory_pointer);
  memory_pointer += text.size();

  int pattern_size = pattern.size();
  std::memcpy(memory_pointer, &pattern_size, sizeof(pattern_size));
  memory_pointer += sizeof(pattern_size);

  std::copy(pattern.begin(), pattern.end(), memory_pointer);
  memory_pointer += pattern.size();

  return data;
}

zmq::message_t fillMessageExecAnswer(std::vector<int>& enrties){
  Header header {
	  toServer,
	  messageExecAnswer,
	  -1,
  };

  int size_of_message = sizeof(Header) + sizeof(int) * enrties.size() + sizeof(int);
  zmq::message_t data(size_of_message);
  char* memory_pointer = (char*)data.data();

  std::memcpy(memory_pointer, &header, sizeof(header));
  memory_pointer += sizeof(header);

  int entries_size = enrties.size();
  std::memcpy(memory_pointer, &entries_size, sizeof(entries_size));
  memory_pointer += sizeof(entries_size);

  std::copy(enrties.begin(), enrties.end(), (int*)memory_pointer);
  memory_pointer += sizeof(int) * enrties.size();

  return data;
}

zmq::message_t fillMessageCreate(int parent_id, int child_id){
  Header header {
	  toClient,
	  messageCreate,
	  parent_id
  };

  CreateBody body {
	  child_id
  };

  int size_of_message = sizeof(Header) + sizeof(body);
  zmq::message_t data(size_of_message);
  char* memory_pointer = (char*)data.data();

  std::memcpy(memory_pointer, &header, sizeof(header));
  memory_pointer += sizeof(header);

  std::memcpy(memory_pointer, &body, sizeof(body));
  memory_pointer += sizeof(body);

  return data;
}

zmq::message_t fillMessageCreateAnswer(int pid, std::string error){
  Header header {
	  toServer,
	  messageCreateAnswer,
	  -1
  };

  int size_of_message = sizeof(Header) + sizeof(pid) + sizeof(int) + error.size() * sizeof(error);
  zmq::message_t data(size_of_message);
  char* memory_pointer = (char*)data.data();

  std::memcpy(memory_pointer, &header, sizeof(header));
  memory_pointer += sizeof(header);

  std::memcpy(memory_pointer, &pid, sizeof(pid));
  memory_pointer += sizeof(pid);

  int error_size = error.size();
  std::memcpy(memory_pointer, &error_size, sizeof(error_size));
  memory_pointer += sizeof(error_size);

  std::copy(error.begin(), error.end(), memory_pointer);
  memory_pointer += error.size();

  return data;
}

zmq::message_t fillMessageRemove(int endPoint){
  Header header {
	  toClient,
	  messageRemove,
	  endPoint
  };

  int size_of_message = sizeof(Header);
  zmq::message_t data(size_of_message);
  char* memory_pointer = (char*)data.data();

  std::memcpy(memory_pointer, &header, sizeof(header));
  memory_pointer += sizeof(header);

  return data;
}

zmq::message_t fillMessagePing(int endPoint){
  Header header {
	  toClient,
	  messagePing,
	  endPoint
  };

  int size_of_message = sizeof(Header);
  zmq::message_t data(size_of_message);
  char* memory_pointer = (char*)data.data();

  std::memcpy(memory_pointer, &header, sizeof(header));
  memory_pointer += sizeof(header);

  return data;
}

zmq::message_t fillMessagePingAnswer(int src_id){
  Header header {
	  toServer,
	  messagePingAnswer,
	  -1
  };

  pingBodyAnswer body {
	  src_id
  };

  int size_of_message = sizeof(Header) + sizeof(body);
  zmq::message_t data(size_of_message);
  char* memory_pointer = (char*)data.data();

  std::memcpy(memory_pointer, &header, sizeof(header));
  memory_pointer += sizeof(header);

  std::memcpy(memory_pointer, &body, sizeof(body));
  memory_pointer += sizeof(body);

  return data;
}

Header* getMessageHeader(zmq::message_t& data){
  return (Header*)data.data();
}

CreateBody getMessageCreate(zmq::message_t& data){
  CreateBody body;
  std::memcpy(&body, (char *)data.data() + sizeof(Header), sizeof(body));
  return body;
}

create_body_answer getMessageCreateAnswer(zmq::message_t& data){
  char* memory_pointer = (char*)data.data() + sizeof(Header);

  int process_id;
  std::memcpy(&process_id, memory_pointer, sizeof(int));
  memory_pointer += sizeof(process_id);

  int error_size;
  std::memcpy(&error_size, memory_pointer, sizeof(int));
  memory_pointer += sizeof(error_size);

  std::string error(memory_pointer, error_size);
  memory_pointer += error_size;

  return {process_id, error};
}

ExecBody getMessageExec(zmq::message_t& data){
  char* memory_pointer = (char*)data.data() + sizeof(Header);

  int text_size;
  std::memcpy(&text_size, memory_pointer, sizeof(int));
  memory_pointer += sizeof(int);

  std::string text(memory_pointer, text_size);
  memory_pointer += text_size;

  int pattern_size;
  std::memcpy(&pattern_size, memory_pointer, sizeof(int));
  memory_pointer += sizeof(int);

  std::string pattern(memory_pointer, pattern_size);
  memory_pointer += pattern_size;

  return {text, pattern};
}

exec_body_answer getMessageExecAnswer(zmq::message_t& data){
  char* memory_pointer = (char*)data.data() + sizeof(Header);

  int entries_size;
  std::memcpy(&entries_size, memory_pointer, sizeof(int));
  memory_pointer += sizeof(int);

  std::vector<int> entries;
  int* integer_pointer = (int*)memory_pointer;
  std::copy(integer_pointer, integer_pointer + entries_size, std::back_inserter(entries));

  return {entries};
}

pingBodyAnswer getMessagePingAnswer(zmq::message_t& data){
  pingBodyAnswer body;
  std::memcpy(&body, (char *)data.data() + sizeof(Header), sizeof(body));
  return body;
}