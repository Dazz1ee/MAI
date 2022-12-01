#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"

void get_file_name(char *buffer) {
    for (int index = 0; index < strlen(buffer); ++index) {
        if (buffer[index] == '\n') {
            buffer[index] = '\0';
            return;
        }
    }
}

int main() {
    shm_unlink(file_name);
    shm_unlink(is_child_name);

    int is_child_id = shm_open(is_child_name, O_CREAT | O_RDWR, mode);
    handle_error(is_child_id == -1,
                 "shm_open error");
    handle_error(ftruncate(is_child_id, size) == -1, "truncate error");
    char* is_child = (char*) mmap(NULL, size,PROT_READ | PROT_WRITE, MAP_SHARED, is_child_id, 0);

    strcpy(is_child, "1");

    int file_descriptor_exchange = shm_open(file_name, O_CREAT | O_RDWR, mode);
    handle_error(ftruncate(file_descriptor_exchange, size) == -1, "truncate error");
    handle_error(file_descriptor_exchange == -1,
                 "shm_open error");

    void* result_data_exchange = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor_exchange, 0);
    handle_error(result_data_exchange == MAP_FAILED, "nmap error");
    strcpy(result_data_exchange, "\0");


    char buffer[50];
    handle_error(read(fileno(stdin), buffer, sizeof(buffer)) <= 0,
                 "error reading form stdin");

    get_file_name(buffer);

    int file_descriptor = open(buffer, O_RDONLY);
    handle_error(file_descriptor == -1, "Can't open file");

    pid_t process_id = fork();
    handle_error(process_id < 0, "process creation error");

    if (process_id == 0) {
        handle_error(dup2(file_descriptor, STDIN_FILENO) < 0, "error dub");
        handle_error(execl("child", file_name, NULL) < 0, "error process");

    } else {
        while(strcmp(is_child, "1") == 0);
        char* buffer_for_float = "start";
        while (strcmp(buffer_for_float, "\0" ) != 0){
            while(strcmp(is_child, "1") == 0);
            buffer_for_float = (char*) result_data_exchange;
            printf("%s\n", buffer_for_float);
            strcpy(is_child, "1");

        }
    }
    shm_unlink(file_name);
    shm_unlink(is_child_name);
    return 0;
}
