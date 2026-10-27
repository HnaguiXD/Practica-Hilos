#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turno = 0;

void *imprimir_descendente(void *arg) {
    for (int i = 10000; i >= 1; i--) {
        pthread_mutex_lock(&mutex);
        
        while (turno != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        
        printf("Hilo secundario: %d\n", i);
        turno = 0;
        
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t hilo;

    if (pthread_create(&hilo, NULL, imprimir_descendente, NULL) != 0) {
        perror("Error al crear el hilo");
        return 1;
    }

    for (int i = 1; i <= 10000; i++) {
        pthread_mutex_lock(&mutex);
        
        while (turno != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        
        printf("Proceso principal: %d\n", i);
        turno = 1;
        
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(hilo, NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
