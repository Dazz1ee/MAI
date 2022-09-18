#include <stdlib.h>
#include <sys/wait.h>
#include "unistd.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

void handle_error(bool expr, char* msg) {
    if (expr) {
        write(fileno(stdout), msg, strlen(msg) * sizeof(char));
        exit(-1);
    }
}

void getFilename(char* buf){
    for (int i = 0; i < strlen(buf); ++i) {
        if(buf[i] == '\n') {
            buf[i] = '\0';
            return;
        }
    }
}

int main() {
    int pipe1[2];
    handle_error((pipe(pipe1) == -1), "pipe error");
    char buf[50];
    handle_error(read(fileno(stdin),buf, sizeof(buf)) <=0, "error reading form stdin");
    getFilename(buf);
    int fd = open(buf, O_RDONLY);
    handle_error(fd == -1, "Can't open file");
    pid_t child = fork();
    if(child == 0){
        close(pipe1[0]);
        handle_error(dup2(fd, STDIN_FILENO) < 0, "error dub");
        char out[50];
        handle_error(sprintf(out, "%d", pipe1[1]) < 0, "error cast");
        handle_error(execl("child", out, NULL) < 0, "error process");
    } else{
        handle_error( (child == -1 ), "process error");
        close(pipe1[1]);
        wait(0);
        float result;
        char ans[50];
        while ((read(pipe1[0], &result, sizeof(float))) > 0) {
            handle_error(result == -1, "div 0");
            sprintf(ans, "%f", result);
            handle_error(write(fileno(stdout), ans, strlen(ans)) == -1, "write error");
            handle_error(write(fileno(stdout), "\n", 1) == -1, "write error n");

        }
    }
    return 0;
}
