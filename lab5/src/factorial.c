
#include <pthread.h>
#include <stdint.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// gcc factorial.c -o factorial && ./factorial --k=996 --mod=997 --pnum=1

typedef struct {
        uint64_t*        result_ptr;
        pthread_mutex_t* mutex;
        uint64_t         mod;
        uint64_t         start_value;
        uint64_t         end_value;
} computation_args;

int get_command_line_args(
        int       argc, 
        char**    argv, 
        uint64_t* k,
        uint64_t* mod,
        uint8_t*  pnum
) {
        static struct option options[] = {{"k", required_argument, 0, 0},
                                          {"mod", required_argument, 0, 0},
                                          {"pnum", required_argument, 0, 0},
                                          {0, 0, 0, 0}};
        while (true) {
                int option_id = 0;
                int c         = getopt_long(argc, argv, "", options, &option_id);
                
                if (c != 0) break;
                
                switch (option_id) {
                        case 0:
                                *k = atoi(optarg);
                                if (*k <= 0) {
                                        printf("k is a positive number\n");
                                        return 1;
                                }
                                break;
                        case 1:
                                *mod = atoi(optarg);
                                if (*mod <= 0) {
                                        printf("mod is a positive number\n");
                                        return 1;
                                }
                                break;
                        case 2:
                                *pnum = atoi(optarg);
                                if (*pnum <= 0) {
                                        printf("pnum is a positive number\n");
                                        return 1;
                                }
                                break;
                }
        }
}

void* compute(void* args_void) {
        computation_args* args = (computation_args*)args_void;
        for (size_t i = args->start_value; i < args->end_value; i++) {
                pthread_mutex_lock(args->mutex);
                *(args->result_ptr) *= i % args->mod;
                *(args->result_ptr) %= args->mod;
                pthread_mutex_unlock(args->mutex);
        }
}

int main(int argc, char** argv) {
        uint64_t k    = 0;
        uint64_t mod  = 0;
        uint8_t  pnum = 0;

        get_command_line_args(argc, argv, &k, &mod, &pnum);
        if (k % pnum != 0) {
                printf("k has to be a multiple of pnum\n");
                return 1;
        }

        struct timeval start_time;
        gettimeofday(&start_time, NULL);


        uint64_t multipliers_per_thread = k / pnum;

        uint64_t result = 1;

        pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_t        threads[pnum];
        computation_args args[pnum];
        for (size_t i = 0; i < pnum; i++) {
                args[i].result_ptr  = &result;
                args[i].mutex       = &mutex;
                args[i].mod         = mod;
                args[i].start_value = multipliers_per_thread * i + 1;
                args[i].end_value   = multipliers_per_thread * (i+1) + 1;

                if (pthread_create(&threads[i], NULL, compute, (void*)&args[i])) {
                        printf("Failed to create a new thread\n");
                        return 1;
                }
        }

        for (size_t i = 0; i < pnum; i++) {
                pthread_join(threads[i], NULL);
        }

        struct timeval finish_time;
        gettimeofday(&finish_time, NULL);
        double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
        elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
        
        printf("Elapsed time: %fms\n", elapsed_time);
        printf("Result: %d", result);
}