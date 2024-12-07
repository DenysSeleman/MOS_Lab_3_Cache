#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <sys/time.h>

#define TOTAL_ITERATIONS 1000000

int counter = 0;
volatile int volatile_counter = 0;
atomic_int atomic_counter = 0;

double current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void* increment(void* args) {
    for (int i = 0; i < TOTAL_ITERATIONS; ++i) {
        counter++;
    }
    return NULL;
}

void* volatile_increment(void* args) {
    for (int i = 0; i < TOTAL_ITERATIONS; ++i) {
        volatile_counter++;
    }
    return NULL;
}

void* atomic_increment(void* args) {
    for (int i = 0; i < TOTAL_ITERATIONS; ++i) {
        atomic_fetch_add(&atomic_counter, 1);
    }
    return NULL;
}

void perform_experiment(void* (*increment_func)(void*), const char* test_name, int num_threads) {
    pthread_t threads[num_threads];
    double start = current_time();

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, increment_func, NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end = current_time();
    printf("%s execution time: %f seconds\n", test_name, end - start);

    if (increment_func == increment) {
        printf("Final counter value: %d\n", counter);
    }
    else if (increment_func == volatile_increment) {
        printf("Final volatile counter value: %d\n", volatile_counter);
    }
    else if (increment_func == atomic_increment) {
        printf("Final atomic counter value: %d\n", atomic_load(&atomic_counter));
    }
}

int main() {
    int num_threads;
    printf("Enter number of threads: ");
    scanf("%d", &num_threads);
    
    perform_experiment(increment, "Increment", num_threads);
    perform_experiment(volatile_increment, "Volatile increment", num_threads);
    perform_experiment(atomic_increment, "Atomic increment", num_threads);
    return 0;
}