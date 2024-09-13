/*
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function


sem_t mutex;       // Semaphore for mutual exclusion for readcount
sem_t wrt;         // Semaphore for mutual exclusion for writing
int readcount = 0; // Number of readers currently reading

// Reader thread function
void *reader(void *arg) {
    int id = *((int *)arg);

    sem_wait(&mutex);
    readcount++;
    if (readcount == 1) {
     
        sem_wait(&wrt);
    }
    sem_post(&mutex);

    // Reading section
    printf("Reader %d is reading\n", id);
    sleep(1); // Simulate reading time

    sem_wait(&mutex);
    readcount--;
    if (readcount == 0) {
        // Last reader unlocks the writer semaphore
        sem_post(&wrt);
    }
    sem_post(&mutex);

    return NULL;
}

// Writer thread function
void *writer(void *arg) {
    int id = *((int *)arg);

    sem_wait(&wrt);
    // Writing section
    printf("Writer %d is writing\n", id);
    sleep(2); // Simulate writing time
    sem_post(&wrt);

    return NULL;
}

int main() {
    pthread_t r_threads[3], w_threads[2];
    int r_ids[3] = {1, 2, 3}; // Reader IDs
    int w_ids[2] = {1, 2};    // Writer IDs

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // Create reader threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&r_threads[i], NULL, reader, &r_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&w_threads[i], NULL, writer, &w_ids[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < 3; i++) {
        pthread_join(r_threads[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(w_threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}


/* 
/q2
Reader 1 is reading
Reader 3 is reading
Reader 2 is reading
Writer 1 is writing
Writer 2 is writing
*/



#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function

// Shared data and synchronization primitives
sem_t mutex;       // Semaphore for mutual exclusion for readcount
sem_t wrt;         // Semaphore for mutual exclusion for writing
int readcount = 0; // Number of readers currently reading

// Reader thread function
void *reader(void *arg) {
    int id = *((int *)arg);

    sem_wait(&mutex);
    readcount++;
    if (readcount == 1) {
        // First reader locks the writer semaphore
        sem_wait(&wrt);
    }
    sem_post(&mutex);

    // Reading section
    printf("Reader %d is reading\n", id);
    sleep(1); // Simulate reading time

    sem_wait(&mutex);
    readcount--;
    if (readcount == 0) {
        // Last reader unlocks the writer semaphore
        sem_post(&wrt);
    }
    sem_post(&mutex);

    return NULL;
}

// Writer thread function
void *writer(void *arg) {
    int id = *((int *)arg);

    sem_wait(&wrt);
    // Writing section
    printf("Writer %d is writing\n", id);
    sleep(2); // Simulate writing time
    sem_post(&wrt);

    return NULL;
}

int main() {
    pthread_t r_threads[5], w_threads[5]; 
    int r_ids[5] = {1, 2, 3, 4, 5}; // Reader IDs
    int w_ids[5] = {1, 2, 3, 4, 5}; // Writer IDs

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // Create reader threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&r_threads[i], NULL, reader, &r_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < 5; i++) { // Changed to 3 writers
        pthread_create(&w_threads[i], NULL, writer, &w_ids[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < 5; i++) {
        pthread_join(r_threads[i], NULL);
    }
    for (int i = 0; i < 5; i++) { // Changed to 3 writers
        pthread_join(w_threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}


/*

./a.out
Reader 1 is reading
Reader 3 is reading
Reader 2 is reading
Reader 4 is reading
Reader 5 is reading
Writer 1 is writing
Writer 2 is writing
Writer 3 is writing
Writer 4 is writing
Writer 5 is writing

*/
