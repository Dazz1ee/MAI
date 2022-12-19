#include <string>
#include <map>
#include "Node.h"
#include <iostream>
#include <fstream>
using namespace std;

#ifndef KP__READER_H_
#define KP__READER_H_

class Reader {
 public:
  Reader(string file_path) {
    file_path_ = file_path;
  }
  map<int, Node> getNodes() {
    ifstream config_file;
    config_file.open(file_path_);

    if(!config_file.is_open()){
      perror("Can't open file");
      exit(1);
    }

    map<int, Node> nodes;

    string line;
    getline(config_file, line);

    if (line != "nodes:") {
      perror("Wrong format file");
    }
    string samples[]{"id: ", "type: ", "children: ", "mutex: ", "file: " };
    string parametrs[5];

    while (getline(config_file, line)) {
      for (int i = 0; i < 5; ++i) {
        getline(config_file, line);
        parametrs[i] = getParameters(samples[i], line);
      }

      int id_node = stoi(parametrs[0]);
      nodes[id_node] = createNode(parametrs);
    }
    return nodes;

  }
 private:
  string file_path_;

  static string getParameters(string sample, string raw_string) {
    int position = raw_string.find(sample);

    if (position != raw_string.length()) {
      return raw_string.substr(position + sample.length());
    } else {
      perror("Wrong format file");
      exit(1);
    }
  }

  Node createNode(string parametrs[]){
    return Node(
        stoi(parametrs[0]),
        parametrs[1],
        converter(parametrs[2], ' '),
        parametrs[3],
        getFiles(parametrs[4] + " ")
    );
  }

  vector<string> getFiles(string raw_string){
    size_t position = 0;
    vector<string> files;

    while ((position = raw_string.find(" ")) != std::string::npos) {
      string file_name = raw_string.substr(0, position);
      files.push_back(file_name);
      raw_string.erase(0, position + 1);
    }

    return files;
  }


  vector<int> converter(string raw_string, char delimeter){
    string id_parent;
    vector<int> parents;
//    cout << raw_string <<  raw_string.length() << endl;
    if(raw_string == "null"){
      return parents;
    }

    int old_index_string = 0;
    int k = 0;
    for (int i = 0; i < raw_string.length(); ++i) {
      if (raw_string[i] == ' '){
        parents.push_back(k);
        k = 0;
        continue;
      }
      k = k * 10 + raw_string[i] - '0';
      if(i == raw_string.length()-1){
        parents.push_back(k);
      }
    }
    return parents;
  }

};

#endif //KP__READER_H_
