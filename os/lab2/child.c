#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


int main(int argc, char* argv[]){
    char c;
    int fd = atoi(argv[0]);
    float res;
    bool flag = true;
    while ((read(fileno(stdin), &c, 1)) >0){
        char* buf = malloc(sizeof(char) * 50);
        int k = 0;
        while(c != ' ' && c != '\n' && c != '\0'){
            buf[k++] = c;
            if(read(fileno(stdin), &c, 1) <= 0) {
                c = EOF;
                break;
            }
        }
        if(flag){
            flag= false;
            res = strtof(buf, &buf);
        }
        else {
            float temp = strtof(buf, &buf);
            if(temp == 0){
                float error = -1;
                write(fd, &error, sizeof(float));
                exit(-1);
            }
            res /= temp;
            if (c == '\n' || c == EOF){
                flag = true;
                write(fd, &res, sizeof(float));
            }
        }
    }
    close(fd);
    return 0;
}