#include "lib.h"

float e(int x) {
    float divisor = 1;
    float answer = 0;
    for (int step = 0; step < x + 1; ++step) {
        answer += 1 / divisor;
        divisor *= step + 1;
    }
    return answer;
}

float square(float first_side, float second_side){
    return (0.5 * first_side * second_side);
}