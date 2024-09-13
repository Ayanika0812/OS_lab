
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // For sleep

#define BUFFER_SIZE 5

int buf[BUFFER_SIZE], f = 0, r = 0;
sem_t mutex, full, empty;

void *produce(void *arg) {
    int i;
    for (i = 0; i < 10; i++) {
        sem_wait(&empty);   // Wait for an empty slot
        sem_wait(&mutex);   // Enter critical section
        buf[r] = i;
        printf("Produced item: %d\n", i);
        r = (r + 1) % BUFFER_SIZE;
        sem_post(&mutex);   // Exit critical section
        sem_post(&full);    // Signal that there is a new item
        sleep(1);           // Simulate time taken to produce
    }
    return NULL;
}

void *consume(void *arg) {
    int item, i;
    for (i = 0; i < 10; i++) {
        sem_wait(&full);    // Wait for a full slot
        sem_wait(&mutex);   // Enter critical section
        item = buf[f];
        printf("Consumed item: %d\n", item);
        f = (f + 1) % BUFFER_SIZE;
        sem_post(&mutex);   // Exit critical section
        sem_post(&empty);   // Signal that there is a new empty slot
        sleep(1);           // Simulate time taken to consume
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);  // Start with 0 full slots
    sem_init(&empty, 0, BUFFER_SIZE);  // Start with BUFFER_SIZE empty slots
    
    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    return 0;
}

/*

./a.out
Produced item: 0
Consumed item: 0
Produced item: 1
Consumed item: 1
Produced item: 2
Consumed item: 2
Produced item: 3
Consumed item: 3
Produced item: 4
Consumed item: 4
Produced item: 5
Consumed item: 5
Produced item: 6
Consumed item: 6
Produced item: 7
Consumed item: 7
Produced item: 8
Consumed item: 8
Produced item: 9
Consumed item: 9
*/
