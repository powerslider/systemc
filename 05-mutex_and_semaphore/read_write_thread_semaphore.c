#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> 

char input[1024];
sem_t len;

void *read1() {
    while(1) {
        printf("\n\nEnter a string: ");
        scanf("%s", input);
        sem_post(&len);
    }
}

void *write1() {
    while(1){
        sem_wait(&len);
        printf("\n\nThe string entered is: %s\n", input);
    }
}

int main() {
    int status;
    pthread_t read_thread, write_thread;

    pthread_create(&read_thread, NULL, read1, NULL);
    pthread_create(&write_thread, NULL, write1, NULL);

    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);
    return 0;
}
