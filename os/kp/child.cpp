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


void generator(char* arguments[]){
  char* file_name = arguments[2];
  FILE* file = fopen(file_name, "a+");

  sleep(1);
  srand(time(NULL));
  int count_number = rand() % 5;
  cout << "generated number : ";
  while (count_number > 0){
    int random_number = rand() % 2;
    fprintf(file, "%d ", random_number);
    cout << random_number << " ";
    --count_number;
  }
  cout << endl;
  fclose(file);
}

void convector(char* arguments[]){
  char* file_name = arguments[2];
  FILE* file = fopen(file_name, "r");

  int number;
  int sum = 0;

  while ((fscanf(file, "%d", &number)) != EOF){
    sum += number;
  }

  fclose(file);
  file = fopen(file_name, "w");
  fprintf(file, "%d ", sum);

  fclose(file);
}

void combiner(int count_argument, char* arguments[]){
  FILE* final_file;

  for (int index_argument = 2; index_argument < count_argument; ++index_argument) {
    if(index_argument == 2){
      final_file = fopen(arguments[2], "a+");
      continue;
    }

    FILE* file = fopen(arguments[index_argument], "r");
    int number;

    while (fscanf(file, "%d",  &number) != EOF){
      fprintf(final_file, "\n %d", number);
    }

    fclose(file);
  }
  fclose(final_file);
}

int main(int argc, char* argv[]){
  if(strcmp(argv[0], "null") != 0) {
    initMutex(argv[0]);
    pthread_mutex_lock(mutex);
  }
  if(strcmp(argv[1], "generator") == 0){
    generator(argv);
  } else if (strcmp(argv[1], "convector") == 0) {
    convector(argv);
  } else {
    combiner(argc,argv);
  }

  if(strcmp(argv[0], "null") != 0) {
    pthread_mutex_unlock(mutex);
  }
}