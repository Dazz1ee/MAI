#include <math.h>
#include "lib.h"

float e(int x) {
    if (x == 0) {
        return 1;
    }
    return powf(1 + 1 / (float) x, x);
}

float square(float first_side, float second_side) {
    return first_side * second_side;
}