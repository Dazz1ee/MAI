#ifndef LAB4_CONSTANTS_H
#define LAB4_CONSTANTS_H

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void handle_error(bool expr, char* msg) {
    if (expr) {
        write(fileno(stderr), msg, strlen(msg) * sizeof(char));
        exit(-1);
    }
}

const char* file_name = "file";
int mode = 0666;
int size = 4096;
const char* is_child_name = "cond";


#endif //LAB4_CONSTANTS_H
