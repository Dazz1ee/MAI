#include <fcntl.h>
#include <sys/mman.h>
#include "Reader.h"
#include "Controller.h"
#include "ExecutableNode.h"
#include "Compare.h"

int main(int argc, char* argv[]) {
  int max_working_job;
  string name_file;
  if(argc != 3){
    perror("enter filename and max_working_job");
    exit(1);
  } else {
    name_file= argv[1];
    max_working_job = stoi(argv[2]);
  }

  Reader reader = Reader(name_file);
  map<int, Node> nodes = reader.getNodes();
  Controller controller = Controller::getInstance();
  controller.setLimit(max_working_job);
  if(!controller.proofCorrect(nodes)){
    perror("DAG not correct");
    exit(1);
  }
  controller.managerController(nodes);

}
