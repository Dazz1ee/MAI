#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void handle_error(bool expr, char* msg) {
    if (expr) {
        write(fileno(stdout), msg, strlen(msg) * sizeof(char));
        exit(-1);
    }
}

void get_file_name(char* buffer){
    for (int i = 0; i < strlen(buffer); ++i) {
        if(buffer[i] == '\n') {
            buffer[i] = '\0';
            return;
        }
    }
}

int main() {
    int pipe1[2];
    handle_error((pipe(pipe1) == -1), "pipe error");
    char buffer[50];
    handle_error(read(fileno(stdin),buffer, sizeof(buffer)) <=0, "error reading form stdin");
    get_file_name(buffer);
    int file_descriptor = open(buffer, O_RDONLY);
    handle_error(file_descriptor == -1, "Can't open file");
    pid_t pid = fork();
    if(pid == 0){
        close(pipe1[0]);
        handle_error(dup2(file_descriptor, STDIN_FILENO) < 0, "error dub");
        char out[50];
        handle_error(sprintf(out, "%d", pipe1[1]) < 0, "error cast");
        handle_error(execl("child", out, NULL) < 0, "error process");
    } else{
        handle_error( (pid == -1 ), "process error");
        close(pipe1[1]);
        wait(0);
        float result;
        char answer[50];
        while ((read(pipe1[0], &result, sizeof(float))) > 0) {
            handle_error(result == -1, "div 0");
            sprintf(answer, "%f", result);
            handle_error(write(fileno(stdout), answer, strlen(answer)) == -1, "write error");
            handle_error(write(fileno(stdout), "\n", 1) == -1, "write error n");
        }
    }
    return 0;
}
