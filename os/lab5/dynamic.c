#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

float (*e_function)(int x) = NULL;
float (*square_function)(float first_side, float second_side) = NULL;

const char* NAIVE_LIBRARY_PATH = "libFirst.so";
const char*  MODERN_LIBRARY_PATH = "libSecond.so";
void *library = NULL;
bool is_first_library = true;

void handle_error(bool expration, char* message) {
    if (expration) {
        perror(message);
        exit(-1);
    }
}

void open_library(const char *path_library) {
    if (library != NULL) {
        dlclose(library);
    }

    library = dlopen(path_library, RTLD_LAZY);
    handle_error(library == NULL, "can't open library");

    e_function = dlsym(library, "e");
    square_function = dlsym(library, "square");
    handle_error(e_function == NULL || square_function == NULL, "Can't find method");
}

void reload_library() {
    is_first_library ? open_library(MODERN_LIBRARY_PATH) : open_library(NAIVE_LIBRARY_PATH);
    is_first_library = !is_first_library;
    printf("library changed\n");
}


int main() {
    open_library(NAIVE_LIBRARY_PATH);
    int query;
    int arg_function1;
    float arg1_function2;
    float arg2_function2;
    while (scanf("%d", &query) != EOF) {
        if (query == 0){
            reload_library();
        } else if (query == 1){
            scanf("%d", &arg_function1);
            printf("Result first function: %f\n", (*e_function)(arg_function1));
        } else if (query == 2){
            scanf("%f %f", &arg1_function2, &arg2_function2);
            printf("Result second function: %f\n", (*square_function)(arg1_function2, arg2_function2));
        } else {
            printf("invalid query\n");
        }
    }
    dlclose(library);
}