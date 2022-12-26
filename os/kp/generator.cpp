#include <iostream>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

pthread_mutex_t* mutex;

void handleError(bool error, char* message){
  if (error) {
    perror(message);
    exit(1);
  }
}

void initMutex(char* mutex_name){
  int descriptor_mutex = shm_open(mutex_name, O_RDWR, 0666);

  handleError(descriptor_mutex < 0, "error shm_open");

  mutex = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t),
                                   PROT_READ | PROT_WRITE, MAP_SHARED, descriptor_mutex, 0);

  handleError(mutex == MAP_FAILED, "nmap error");
}


void generator(int argc, char* arguments[]){
  char* meta_file_name = (char*) ("out/METAINF" + (string) arguments[1]).c_str();
  FILE* meta_file = fopen(meta_file_name, "w");
  char* out_file_name = (char*) ("out/number" + (string) arguments[1]).c_str();
  FILE* out_file = fopen(out_file_name, "w");
  srand(time(NULL));
  int count_number = rand() % 5;
  cout << "generated number : ";
  while (count_number > 0){
    int random_number = rand() % 2;
    fprintf(out_file, "%d ", random_number);
    cout << random_number << " ";
    --count_number;
  }
  cout << endl;
  fclose(out_file);
  fprintf(meta_file, "%s ", out_file_name);
  fclose(meta_file);
}


int main(int argc, char* argv[]){
  if(strcmp(argv[0], "null") != 0) {
    initMutex(argv[0]);
    pthread_mutex_lock(mutex);
  }
  cout << "Runnig node with id: " << argv[1] << endl;
  generator(argc, argv);

  if(strcmp(argv[0], "null") != 0) {
    pthread_mutex_unlock(mutex);
  }
}