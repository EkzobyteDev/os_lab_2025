
#include <pthread.h>
#include <stdio.h>

typedef struct
{
        pthread_mutex_t* a;
        pthread_mutex_t* b;
} thread_data;


void* thread_1_function(void* args) {
        thread_data* data = (thread_data*)args;
        
        printf("Thread 1: waiting for mutex a to unlock...\n");
        pthread_mutex_lock(data->a);
        printf("Thread 1: mutex a is locked\n");

        printf("Thread 1: waiting for mutex b to unlock...\n");
        pthread_mutex_lock(data->b);
        printf("Thread 1: mutex b is locked\n");

        pthread_mutex_unlock(data->a);
        pthread_mutex_unlock(data->b);
        printf("Thread 1: mutexes a & b are unlocked\n");
}

void* thread_2_function(void* args) {
        thread_data* data = (thread_data*)args;
        
        printf("Thread 2: waiting for mutex b to unlock...\n");
        pthread_mutex_lock(data->b);
        printf("Thread 2: mutex b is locked\n");

        printf("Thread 2: waiting for mutex a to unlock...\n");
        pthread_mutex_lock(data->a);
        printf("Thread 2: mutex a is locked\n");

        pthread_mutex_unlock(data->a);
        pthread_mutex_unlock(data->b);
        printf("Thread 2: mutexes a & b are unlocked\n");
}

int main() {
        pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_t b = PTHREAD_MUTEX_INITIALIZER;

        pthread_t   threads[2];
        thread_data data[2];

        for (int i = 0; i < 2; i++) {
                data[i].a = &a;
                data[i].b = &b;
        }

        pthread_create(&(threads[0]), NULL, thread_1_function, (void*)&(data[0]));
        pthread_create(&(threads[0]), NULL, thread_2_function, (void*)&(data[0]));

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
}