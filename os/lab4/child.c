#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"

int main(int argc, char* argv[]){
    int is_child_id = shm_open(is_child_name, O_CREAT | O_RDWR, mode);
    handle_error(is_child_id == -1,
                 "shm_open error");
    handle_error(ftruncate(is_child_id, size) == -1, "truncate error");
    char* is_child = (char*) mmap(
            NULL,
            size,
            PROT_READ | PROT_WRITE,
            MAP_SHARED, is_child_id,
            0);

    int file_descriptor_exchange = shm_open(file_name, O_CREAT | O_RDWR, mode);
    handle_error(ftruncate(file_descriptor_exchange, size) == -1, "truncate error");
    handle_error(file_descriptor_exchange == -1,
                 "shm_open error");

    void* pointer_data_exchange = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor_exchange, 0);

    char character;
    float result;
    bool is_first_number = true;

    while ((read(fileno(stdin), &character, 1)) >0){
        char* buffer = malloc(sizeof(char) * 50);
        int buffer_index = 0;
        while(character != ' ' && character != '\n' && character != '\0'){
            buffer[buffer_index++] = character;
            if(read(fileno(stdin), &character, 1) <= 0) {
                character = EOF;
                break;
            }
        }

        if(is_first_number){
            is_first_number = false;
            result = strtof(buffer, &buffer);
        } else {
            float number = strtof(buffer, &buffer);

            if(number == 0){
                strcpy(pointer_data_exchange, "-1");
                exit(-1);
            }
            result /= number;

            if (character == '\n' || character == EOF){
                is_first_number = true;
                while(strcmp(is_child,"0") == 0);
                sprintf(pointer_data_exchange, "%f", result);
                strcpy(is_child, "0");

            }
        }
    }

    while(strcmp(is_child,"0") == 0);
    strcpy(pointer_data_exchange, "\0");
    strcpy(is_child, "0");

    return 0;
}