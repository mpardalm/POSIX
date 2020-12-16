#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define PLAZAS 5
#define NUM_COCHES 8

sem_t espera, entrada;
sigset_t sigset;

void *parking (void *arg) {
    int id = *(int*) arg;
    while (1) {
        sem_wait(&entrada);
        printf("Llegando al parking el coche: %d\n", id);
        fflush(stdout);
        kill(getpid(), 0);
        sem_wait(&espera);
        printf("Aparcando coche: %d\n", id);
        fflush(stdout);
        sem_post(&entrada);
        sleep(ran() % 15);
        printf("Saliendo coche: %d\n", id);
        fflush(stdout);
        kill(getpid(), 1);
        sleep(5);
    }
    pthread_exit(NULL);
}

void *controladora() {
    int info;
    int cont = 0;
    int esperando = 0;
    while (1) {
        sigwait(&sigset, &info);
        if (info == 0 && cont < PLAZAS) {
            sem_post(&espera);
            cont++;
        } else if (info == 0) {
            esperando = 1;
            printf("Parking lleno\n");
            fflush(stdout);
        } else if (info == 1) {
            cont--;
            printf("Saliendo vehículo\n");
            fflush(stdout);
            if (esperando == 1) {
                esperando = 0;
                cont++;
                sem_post(&espera);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    sigemptyset(&sigset);
    sigaddset(&sigset, 0);
    sigaddset(&sigset, 1);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);

    sem_init(&espera, 0, 0);
    sem_init(&entrada, 0, 1);

    pthread_t coches[NUM_COCHES], controlador;
    int id[NUM_COCHES];

    for (int i = 0; i < NUM_COCHES; i++) {
        id[i] = i;
        pthread_create(&coches[i], NULL, parking, &id[i]);
    }
    pthread_create(&controlador, NULL, controladora, NULL);

    for (int i = 0; i < NUM_COCHES; i++) {
        pthread_join(coches[i], NULL);
    }
    pthread_join(controlador, NULL);
    return 0;
}