#include <iostream>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

pthread_mutex_t* mutex;

void handleError(bool error, char* message) {
  if (error) {
	perror(message);
	exit(1);
  }
}

void initMutex(char* mutex_name) {
  int descriptor_mutex = shm_open(mutex_name, O_RDWR, 0666);

  handleError(descriptor_mutex < 0, "error shm_open");

  mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t),
								 PROT_READ | PROT_WRITE, MAP_SHARED, descriptor_mutex, 0);

  handleError(mutex == MAP_FAILED, "nmap error");
}

void convector(int count_argument, char* arguments[]) {
  char* meta_file_name = (char*)("out/METAINF" + (string)arguments[1]).c_str();
  FILE* meta_file = fopen(meta_file_name, "w");

  char* out_file_name = (char*)malloc(255);
  strcpy(out_file_name, ("out/number" + (string)arguments[1]).c_str());

  FILE* out_file = fopen(out_file_name, "w");

  int sum = 0;
  for (int index_argument = 2; index_argument < count_argument; ++index_argument) {
	char* info_file_name = (char*)malloc(255);
	info_file_name = (char*)("out/METAINF" + (string)arguments[index_argument]).c_str();
	FILE* info_file = fopen(info_file_name, "r");

	char* name_input_file = (char*)malloc(255);

	while (fscanf(info_file, "%s", name_input_file) != EOF) {
	  FILE* input_file = fopen(name_input_file, "r");
	  int count;

	  while (fscanf(input_file, "%d", &count) != EOF) {
		sum += count;
	  }

	  fclose(input_file);
	}
	fclose(info_file);
  }
  fprintf(out_file, "%d ", sum);
  fclose(out_file);
  fprintf(meta_file, "%s ", out_file_name);
  fclose(meta_file);
}

int main(int argc, char* argv[]) {
  if (strcmp(argv[0], "null") != 0) {
	initMutex(argv[0]);
	pthread_mutex_lock(mutex);
  }
  cout << "Runnig node with id: " << argv[1] << endl;
  convector(argc, argv);

  if (strcmp(argv[0], "null") != 0) {
	pthread_mutex_unlock(mutex);
  }
}