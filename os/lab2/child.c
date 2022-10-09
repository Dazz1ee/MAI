#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


int main(int argc, char* argv[]){
    int file_descriptor = atoi(argv[0]);
    char character;
    float result;
    bool is_first_number = true;
    while ((read(fileno(stdin), &character, 1)) >0){
        char* buffer = malloc(sizeof(char) * 50);
        int k = 0;
        while(character != ' ' && character != '\n' && character != '\0'){
            buffer[k++] = character;
            if(read(fileno(stdin), &character, 1) <= 0) {
                character = EOF;
                break;
            }
        }
        if(is_first_number){
            is_first_number = false;
            result = strtof(buffer, &buffer);
        }
        else {
            float number = strtof(buffer, &buffer);
            if(number == 0){
                float error = -1;
                write(file_descriptor, &error, sizeof(float));
                exit(-1);
            }
            result /= number;
            if (character == '\n' || character == EOF){
                is_first_number = true;
                write(file_descriptor, &result, sizeof(float));
            }
        }
    }
    close(file_descriptor);
    return 0;
}