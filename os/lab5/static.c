#include <stdio.h>
#include "lib.h"

int main() {
    int query;
    int counter;
    float first_side;
    float second_side;
    while (scanf("%d", &query) != EOF) {
        if (query == 1) {
            scanf("%d", &counter);
            printf("Result first function: %f\n", e(counter));
        } else if (query == 2) {
            scanf("%f %f",&first_side, &second_side);
            printf("Result second function: %f\n", square(first_side, second_side));
        } else {
            printf("invalid query\n");
        }
    }
}
