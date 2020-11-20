#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *PrintHello(void *threadId) {
    long tId = *(long*) threadId;
    printf("Hello World! It's me, thread #%ld!\n", tId);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    long id[NUM_THREADS];

    for (t = 0; t < NUM_THREADS; t++) {
        id[t] = t;
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, &id[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
}