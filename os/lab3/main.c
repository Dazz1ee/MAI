#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int* array;
    int start_array;
    int end_array;
    int* minimum;
} thread_data;

pthread_mutex_t mutex;

void handle_error(bool expression, char* message) {
    if (expression) {
        write(fileno(stdout), message, strlen(message) * sizeof(char));
        exit(-1);
    }
}

void print_result(int minimum, int proof, int time, char* number_threads){
    char minimum_string[50];
    char proof_string[50];
    char time_string[50];
    sprintf(minimum_string, "%d", minimum);
    sprintf(proof_string, "%d", proof);
    sprintf(time_string, "%d", time);
    write(fileno(stdout), "number of thread = ", strlen("number of thread = "));
    write(fileno(stdout), number_threads, strlen(number_threads));
    write(fileno(stdout), "\n", 1);
    write(fileno(stdout), "time = ", strlen("time = "));
    write(fileno(stdout), time_string, strlen(time_string));
    write(fileno(stdout), "\n", 1);
    write(fileno(stdout), "minimum = ", strlen("minimum = "));
    write(fileno(stdout), minimum_string, strlen(minimum_string));
    write(fileno(stdout), "\n", 1);
    write(fileno(stdout), "proof = ", strlen("proof = "));
    write(fileno(stdout), proof_string, strlen(proof_string));
    write(fileno(stdout), "\n", 1);
}

void* function_thread(void* parametrs){
    thread_data* data = (thread_data*) parametrs;
    int minimum = data->array[data->start_array];
    for(int i = data->start_array; i < data->end_array; i++){
        if(data->array[i] < minimum){
            minimum = data->array[i];
        }
    }
    pthread_mutex_lock(&mutex);
    if(minimum < *(data->minimum)){
        *(data->minimum) = minimum;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char** argv){
    int max_thread = sysconf(_SC_NPROCESSORS_ONLN);
    handle_error(argc < 2, "write the number of threads");
    int number_threads = atoi(argv[1]);
    char max_threads_string[50];
    sprintf(max_threads_string, "%d", number_threads);
    if(number_threads > max_thread){
        number_threads = max_thread;
        sprintf(max_threads_string, "%d", number_threads);
        write(fileno(stdout), "flow limit exceeded, installed ", strlen("flow limit exceeded, installed "));
        write(fileno(stdout), max_threads_string, strlen(max_threads_string));
        write(fileno(stdout), "\n", 1);
    }
    pthread_t* threads = (pthread_t*) malloc(number_threads * sizeof(pthread_t));
    pthread_mutex_init(&mutex,NULL);
    int size_array = 1000000 + rand() % 1000000;
    int* array = (int*) malloc(size_array * sizeof(int));
    for (int i = 0; i < size_array; ++i) {
        array[i] = rand();
    }
    int minimum = array[0];
    thread_data* array_data = (thread_data*) malloc(number_threads * sizeof(thread_data));
    for (int i = 0; i < number_threads; ++i) {
        array_data[i].array = array;
        array_data[i].start_array = size_array / number_threads * i;
        array_data[i].end_array = size_array / number_threads * (i+1);
        array_data[i].minimum = &minimum;
        if(i == number_threads - 1){
            array_data[i].end_array += size_array % number_threads;
        }
    }
    struct timeval start_time, stop_time;
    handle_error(gettimeofday(&start_time, NULL), "Time set error");
    for (int i = 0; i < number_threads; ++i) {
        handle_error(pthread_create(&(threads[i]), NULL, function_thread,  &array_data[i]) != 0, "Error. Don't create thread");
    }
    for (int i = 0; i < number_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    handle_error(gettimeofday(&stop_time, NULL) == -1, "Time set error");
    pthread_mutex_destroy(&mutex);
    int proof = array[0];
    for (int i = 0; i < size_array; ++i) {
        if(proof > array[i]){
            proof = array[i];
        }
    }
    free(threads);
    free(array_data);
    free(array);
    int time = (stop_time.tv_sec - start_time.tv_sec) * 1000 + stop_time.tv_usec - start_time.tv_usec;
    print_result(minimum,proof,time, max_threads_string);
    return 0;
}

