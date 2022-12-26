#include <map>
#include <queue>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include "Node.h"
#include "ExecutableNode.h"

using namespace std;

#ifndef KP__CONTROLLER_H_
#define KP__CONTROLLER_H_

class Controller {
 public:
  bool proofCorrect(map<int, Node> &nodes) {
	vector<int> visited(nodes.size() + 1, 0);
	vector<int> startingNodes = getStartingNodes(nodes);
	if (startingNodes.empty()) {
	  return false;
	}
	for (int node : startingNodes) {
	  if (dfs(node, visited, nodes)) {
		return false;
	  }
	}
	return checkConnectedness(nodes);
  }

  void managerController(map<int, Node> &nodes) {
	closeMappingFiles(nodes);
	initMutex(nodes);
	parents_id.resize(nodes.size() + 1);
	if (stat("out", NULL) == -1) {
	  mkdir("out", ACCESSPERMS);
	}
	bfs(nodes);
	count_exited_nodes_ = 0;
	count_working_nodes_ = 0;
	map<int, ExecutableNode> pid_working_jobs;

	while (count_exited_nodes_ < executable_nodes_.size()) {
	  if (count_working_nodes_ < limit_process_) {
		executeJob(pid_working_jobs, nodes);
	  }

	  int status;
	  int process_id = waitpid(-1, &status, 0);

	  handleError(WEXITSTATUS(status) == 1, "node finish with error");
	  if (WIFEXITED(status) == false) {
		continue;
	  }

	  cout << "node with id: " << pid_working_jobs[process_id].node.getId() << " complete" << endl;
	  for (int child_id : pid_working_jobs[process_id].node.getChildrenId()) {
		parents_id[child_id].push_back(pid_working_jobs[process_id].node.getId());
		executable_nodes_[child_id].priority--;
	  }
	  count_exited_nodes_++;
	  count_working_nodes_--;

	}
	closeMappingFiles(nodes);
  }

  static Controller &getInstance() {
	static Controller instance;
	return instance;
  }

  void setLimit(int limit_process) {
	limit_process_ = limit_process;
  }

 private:

  Controller() {
  }

  int limit_process_;
  int count_exited_nodes_;
  int count_working_nodes_;
  map<int, ExecutableNode> executable_nodes_;
  vector<vector<int>> parents_id;

  void initMutex(map<int, Node> nodes) {
	for (map<int, Node>::iterator iterator_node = nodes.begin();
		 iterator_node != nodes.end();
		 ++iterator_node) {
	  if (iterator_node->second.getMutex() == "null") {
		continue;
	  }

	  char* mutex_name = (char*)iterator_node->second.getMutex().c_str();
	  int descriptor_mutex = shm_open(mutex_name, O_EXCL | O_CREAT | O_RDWR | O_TRUNC, 0666);

	  if (descriptor_mutex < 0) {
		continue;
	  }
	  handleError(ftruncate(descriptor_mutex, sizeof(pthread_mutex_t)) == -1, "truncate error");

	  pthread_mutex_t* mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t),
													  PROT_READ | PROT_WRITE, MAP_SHARED, descriptor_mutex, 0);

	  handleError(mutex == MAP_FAILED, "nmap error");

	  pthread_mutexattr_t mutex_attribute;
	  pthread_mutexattr_init(&mutex_attribute);
	  pthread_mutexattr_setpshared(&mutex_attribute, PTHREAD_PROCESS_SHARED);
	  pthread_mutex_init(mutex, &mutex_attribute);
	  pthread_mutexattr_destroy(&mutex_attribute);
	}
  }

  void executeJob(map<int, ExecutableNode> &pid_working_jobs, map<int, Node> nodes) {
	for (map<int, ExecutableNode>::iterator iterator_nodes = executable_nodes_.begin();
		 iterator_nodes != executable_nodes_.end();
		 ++iterator_nodes) {

	  if (count_working_nodes_ >= limit_process_) {
		return;
	  }

	  if (iterator_nodes->second.priority == 0 && !checkExec(pid_working_jobs, iterator_nodes->second)) {
		count_working_nodes_++;
		int process_id = fork();

		if (process_id == 0) {
		  char* buffer_argument[3 + parents_id[iterator_nodes->second.node.getId()].size()];

		  buffer_argument[0] = (char*)iterator_nodes->second.node.getMutex().c_str();
		  buffer_argument[1] = (char*)to_string(iterator_nodes->second.node.getId()).c_str();
		  int counter = 0;

		  for (int parent : parents_id[iterator_nodes->second.node.getId()]) {
			char* integer_string = (char*)malloc(12);
			sprintf(integer_string, "%d", parent);
			buffer_argument[2 + counter] = integer_string;
			counter++;
		  }

		  buffer_argument[2 + counter] = NULL;

		  if (execv((char*)iterator_nodes->second.node.getPath().c_str(), buffer_argument) < 0) {
			perror("Exec error");
			exit(1);
		  }
		}
		pid_working_jobs[process_id] = iterator_nodes->second;
	  }
	}
  }

  vector<int> getStartingNodes(map<int, Node> &nodes) {
	vector<int> is_starting_node(nodes.size() + 1, 1);

	for (map<int, Node>::iterator node = nodes.begin(); node != nodes.end(); ++node) {
	  for (int child_id : node->second.getChildrenId()) {
		is_starting_node[child_id] = 0;
	  }
	}

	vector<int> starting_nodes(0);

	for (int i = 1; i < nodes.size() + 1; ++i) {
	  if (is_starting_node[i]) {
		starting_nodes.push_back(i);
	  }
	}
	return starting_nodes;
  }

  bool dfs(int node, vector<int> &visited, map<int, Node> &nodes) {
	visited[node] = 1;

	for (int i = 0; i < nodes[node].getChildrenId().size(); ++i) {
	  int to = nodes[node].getChildrenId()[i];

	  if (visited[to] == 0) {
		if (dfs(to, visited, nodes)) {
		  return true;
		}
	  } else if (visited[to] == 1) {
		return true;
	  }
	}
	visited[node] = 2;
	return false;
  }

  void bfs(map<int, Node> &nodes) {
	vector<int> starting_nodes = getStartingNodes(nodes);
	queue<int> queue_nodes;
	map<int, ExecutableNode> priority_nodes;
	vector<int> visited(nodes.size() + 1, 0);

	for (int node : starting_nodes) {
	  queue_nodes.push(node);
	  priority_nodes[node] = {nodes[node], 0, -1};
	}

	while (!queue_nodes.empty()) {
	  int node_id = queue_nodes.front();
	  queue_nodes.pop();
	  for (int child_id : nodes[node_id].getChildrenId()) {
		if (visited[child_id] == 0) {
		  visited[child_id] = 1;
		  priority_nodes[child_id] = {nodes[child_id], 1, -1};
		  queue_nodes.push(child_id);
		} else {
		  priority_nodes[child_id].priority += 1;
		}
	  }
	}
	executable_nodes_ = priority_nodes;
  }

  bool checkExec(map<int, ExecutableNode> &pid_working_nodes, ExecutableNode node) {
	for (map<int, ExecutableNode>::iterator iterator_nodes = pid_working_nodes.begin();
		 iterator_nodes != pid_working_nodes.end();
		 ++iterator_nodes) {
	  if (iterator_nodes->second == node) {
		return true;
	  }
	}
	return false;
  }

  void closeMappingFiles(map<int, Node> &nodes) {
	for (map<int, Node>::iterator iterator_node = nodes.begin();
		 iterator_node != nodes.end();
		 ++iterator_node) {
	  if (iterator_node->second.getMutex() != "null") {
		shm_unlink(iterator_node->second.getMutex().c_str());
	  }
	}
  }

  void handleError(bool error, char* message) {
	if (error) {
	  perror(message);
	  exit(1);
	}
  }

  bool checkConnectedness(map<int, Node> &nodes) {
	vector<vector<int>> graph = getGraph(nodes);

	queue<int> queue_nodes;
	vector<int> visited(nodes.size() + 1, 0);
	queue_nodes.push(1);
	visited[1] = 1;

	while (!queue_nodes.empty()) {
	  int node_id = queue_nodes.front();
	  queue_nodes.pop();
	  for (int child_id : graph[node_id]) {
		if (visited[child_id] == 0) {
		  visited[child_id] = 1;
		  queue_nodes.push(child_id);
		}
	  }
	}

	for (int i = 1; i < visited.size(); ++i) {
	  if (visited[i] == 0) {
		return false;
	  }
	}
	return true;
  }

  vector<vector<int>> getGraph(map<int, Node> &nodes) {
	vector<vector<int>> graph(nodes.size() + 1);

	for (map<int, Node>::iterator iterator_node = nodes.begin();
		 iterator_node != nodes.end();
		 ++iterator_node) {
	  for (int children_id : iterator_node->second.getChildrenId()) {
		graph[iterator_node->first].push_back(children_id);
		graph[children_id].push_back(iterator_node->first);
	  }
	}
	return graph;
  }

};

#endif //KP__CONTROLLER_H_
