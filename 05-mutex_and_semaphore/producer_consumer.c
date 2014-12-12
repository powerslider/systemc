#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define BUFF_SIZE 5 // total number of slots
#define NP 3 // total number of producers
#define NC 3 // total number of consumers
#define NITERS 4 // number of items produced/consumed

typedef struct {
    int buf[BUFF_SIZE]; // shared var
    int in; // buf[in % BUFF_SIZE] is the first empty slot
    int out; // buf[out % BUFF_SIZE] is the first full slot
    sem_t full; // keep track of the number of full slots
    sem_t empty; // keep track of the number of empty slots
    sem_t mutex; // enforce mutual exclusion to shared data
} shared_buff_t;

shared_buff_t shared;

void* producer(void* arg) {
    int i, item, index;
    index = atoi(arg);
    printf("Starting Producer %d\n", index);
    
    for (i =0; i < NITERS; i++) {
        // Produce item.
        item = i;
        // Prepare to write item to buf
        // if there are no empty slots, wait
        sem_wait(&shared.empty);
        // If another thread uses the buffer, wait
        sem_wait(&shared.mutex);
        shared.buf[shared.in] = item;
        shared.in = (shared.in + 1) % BUFF_SIZE;
        printf("[P%d] + Producing %d ... \n", index, item);
        fflush(stdout);
        // Release the buffer
        sem_post(&shared.mutex);
        // Increment the number of full slots
        sem_post(&shared.full);

        // Interleave producer and consumer execution
        if (i % 2 == 1) {
            sleep(1);
        }
    }
    
    printf("Producer %li ended.\n", (long)arg);
    
    return (void*)arg;
}

void* consumer(void* arg) {
    int i = 0;
    int value = 0;
    int index = atoi(arg);
    for (i =0; i < NITERS; i++) {
        sem_wait(&shared.full);
        sem_wait(&shared.mutex);
        value = shared.buf[shared.out];
        shared.buf[shared.out] = 0;
        shared.out = (shared.out + 1) % BUFF_SIZE;
        printf("[C%d] Consuming %d .. with value %d\n", index, i, value);
        fflush(stdout);
        sem_post(&shared.mutex);
        sem_post(&shared.empty);

        if (i % 2 == 1) {
            sleep(1);
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t producer_thread_id, consumer_thread_id;
    intptr_t index;
    bzero(&shared, sizeof(shared));
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    sem_init(&shared.mutex, 0, 1);
    
    for (index = 0; index < NP; index++) {
        // Create new producer
        pthread_create(&producer_thread_id, NULL, producer, (void *)index);
    }

    for (index = 0; index < NC; index++) {
        //Create new consumer
        pthread_create(&consumer_thread_id, NULL, consumer, (void *)index);
    }
    
    pthread_exit(NULL);
    return 0;
}
