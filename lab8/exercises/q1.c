/*
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // For sleep

#define BUFFER_SIZE 5
#define MAX_DIFF 10

int buf[BUFFER_SIZE], f = 0, r = 0;
sem_t mutex, full, empty, limit_sem;
int produced_count = 0, consumed_count = 0;

void *produce(void *arg) {
    int i;
    for (i = 0; i < 10; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        
        // Check if the difference between produced and consumed items exceeds MAX_DIFF
        while ((produced_count - consumed_count) >= MAX_DIFF) {
            sem_post(&mutex);  // Release mutex before waiting on limit_sem
            sem_wait(&limit_sem);  // Wait until the difference is below MAX_DIFF
            sem_wait(&mutex);  // Re-acquire mutex
        }
        
        // Produce the item
        printf("Produced item: %d\n", i);
        buf[r] = i;
        r = (r + 1) % BUFFER_SIZE;
        produced_count++;
        
        sem_post(&mutex);
        sem_post(&full);
        
        // Print semaphore values (for debugging)
        int full_value, empty_value;
        sem_getvalue(&full, &full_value);
        sem_getvalue(&empty, &empty_value);
        printf("Full semaphore value: %d\n", full_value);
        printf("Empty semaphore value: %d\n", empty_value);
        
        sleep(1);  // Simulate time taken to produce
    }
    return NULL;
}

void *consume(void *arg) {
    int item, i;
    for (i = 0; i < 10; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        
        // Consume the item
        item = buf[f];
        f = (f + 1) % BUFFER_SIZE;
        consumed_count++;
        printf("Consumed item: %d\n", item);
        
        // Signal if the difference between produced and consumed items is below MAX_DIFF
        if ((produced_count - consumed_count) < (MAX_DIFF - 1)) {
            sem_post(&limit_sem);
        }
        
        sem_post(&mutex);
        sem_post(&empty);
        
        // Print semaphore values (for debugging)
        int full_value, empty_value;
        sem_getvalue(&full, &full_value);
        sem_getvalue(&empty, &empty_value);
        printf("Full semaphore value: %d\n", full_value);
        printf("Empty semaphore value: %d\n", empty_value);
        
        sleep(1);  // Simulate time taken to consume
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);   // Initially no items are in the buffer
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all slots are empty
    sem_init(&limit_sem, 0, 0);  // Start with limit semaphore at 0
    
    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&limit_sem);
    
    return 0;
}
*/


#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // For sleep function

#define BUFFER_SIZE 5
#define MAX_DIFF 10

int buf[BUFFER_SIZE], f = 0, r = 0;
sem_t mutex, full, empty, limit_sem;
int produced_count = 0, consumed_count = 0;

void *produce(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {  // Produce 20 items
        sem_wait(&empty);        // Wait until there is an empty slot
        sem_wait(&mutex);        // Lock access to shared data

        // Enforce the limit: check if we can produce more
        while ((produced_count - consumed_count) >= MAX_DIFF) {
            sem_post(&mutex);    // Release mutex before waiting on limit_sem
            sem_wait(&limit_sem); // Wait until the consumer allows more production
            sem_wait(&mutex);    // Re-acquire mutex
        }

        // Produce the item
        printf("Produced item: %d\n", i);
        buf[r] = i;
        r = (r + 1) % BUFFER_SIZE;  // Update rear index
        produced_count++;

        sem_post(&mutex);        // Unlock access to shared data
        sem_post(&full);         // Signal that there is a new item to consume
        
        sleep(1);  // Simulate production time
    }
    return NULL;
}

void *consume(void *arg) {
    int item, i;
    for (i = 0; i < 20; i++) {  // Consume 20 items
        sem_wait(&full);         // Wait until there is an item to consume
        sem_wait(&mutex);        // Lock access to shared data

        // Consume the item
        item = buf[f];
        f = (f + 1) % BUFFER_SIZE;  // Update front index
        consumed_count++;
        printf("Consumed item: %d\n", item);

        // Allow the producer to produce more if the difference is within the limit
        if ((produced_count - consumed_count) < MAX_DIFF) {
            sem_post(&limit_sem);  // Signal the producer to produce more
        }

        sem_post(&mutex);        // Unlock access to shared data
        sem_post(&empty);        // Signal that there is an empty slot in the buffer

        sleep(1);  // Simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);    // Initially no items are in the buffer
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all slots are empty
    sem_init(&limit_sem, 0, 0);  // Start with limit semaphore at 0

    // Create producer and consumer threads
    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume, NULL);

    // Wait for both threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&limit_sem);

    return 0;
}

