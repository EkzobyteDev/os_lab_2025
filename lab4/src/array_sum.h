#pragma once 

typedef struct {
        int *array;
        int begin;
        int end;
} SumArgs;

int Sum(const SumArgs* args);