#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/time.h>
#include "utils.h"
#include "array_sum.h"

int GetCommandLineArgs(
        int       argc, 
        char**    argv, 
        uint32_t* threads_num, 
        uint32_t* array_size, 
        uint32_t* seed
) {
        static struct option options[] = {{"seed", required_argument, 0, 0},
                                          {"array_size", required_argument, 0, 0},
                                          {"threads_num", required_argument, 0, 0},
                                          {0, 0, 0, 0}};
        while (true) {
                int option_id = 0;
                int c         = getopt_long(argc, argv, "", options, &option_id);
                
                if (c != 0) break;
                
                switch (option_id) {
                        case 0:
                        *seed = atoi(optarg);
                        if (*seed <= 0) {
                                printf("seed is a positive number\n");
                                        return 1;
                                }
                                break;
                        case 1:
                        *array_size = atoi(optarg);
                        if (*array_size <= 0) {
                                printf("array_size is a positive number\n");
                                return 1;
                        }
                        break;
                        case 2:
                        *threads_num = atoi(optarg);
                        if (*threads_num <= 0) {
                                printf("threads_num is a positive number\n");
                                return 1;
                        }
                        if (*array_size % *threads_num != 0) {
                                printf("array_size must be a multiple of threads_num\n");
                                return 1;
                        }
                        break;
                }
        }
}

void* ThreadSum(void* args) {
        SumArgs *sum_args = (SumArgs*)args;
        return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char** argv) {
        uint32_t threads_num = 0;
        uint32_t array_size  = 0;
        uint32_t seed        = 0;

        GetCommandLineArgs(argc, argv, &threads_num, &array_size, &seed);

        uint32_t elements_per_thread = array_size / threads_num;
        
        pthread_t threads[threads_num];
        int *array = malloc(sizeof(int) * array_size);
        GenerateArray(array, array_size, seed);
        
        struct timeval start_time;
        gettimeofday(&start_time, NULL);

        SumArgs args[threads_num];
        for (uint32_t i = 0; i < threads_num; i++) {
                args[i].array = array;
                args[i].begin = elements_per_thread * i;
                args[i].end   = elements_per_thread * (i+1);

                if (pthread_create(&threads[i], NULL, ThreadSum, (void*)&(args[i]))) {
                        printf("Error: pthread_create failed!\n");
                        return 1;
                }
        }

        size_t total_sum = 0;
        for (uint32_t i = 0; i < threads_num; i++) {
                size_t sum = 0;
                pthread_join(threads[i], (void**)&sum);
                total_sum += sum;
        }

        struct timeval finish_time;
        gettimeofday(&finish_time, NULL);

        double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
        elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

        free(array);
        printf("Total: %d\n", total_sum);
        printf("Elapsed time: %fms\n", elapsed_time);
        return 0;
}
