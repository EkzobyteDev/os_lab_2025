#include "array_sum.h"

int Sum(const SumArgs* args) {
        int sum = 0;
        for (int i = args->begin; i < args->end; i++) {
                sum += args->array[i];
        }
        return sum;
}