/*
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function

// Shared semaphores
sem_t semaphore1;
sem_t semaphore2;

// Function for Thread 1
void *thread1_func(void *arg) {
    printf("Thread 1: Trying to acquire semaphore1...\n");
    sem_wait(&semaphore1);
    printf("Thread 1: Acquired semaphore1, trying to acquire semaphore2...\n");
    
    // Simulate some processing
    sleep(2); // Increased sleep to ensure both threads reach deadlock

    sem_wait(&semaphore2);
    printf("Thread 1: Acquired semaphore2.\n");

    // Simulate some processing
    sleep(2);

    // Release the semaphores
    sem_post(&semaphore2);
    sem_post(&semaphore1);
    
    printf("Thread 1: Released semaphore2 and semaphore1.\n");

    return NULL;
}

// Function for Thread 2
void *thread2_func(void *arg) {
    printf("Thread 2: Trying to acquire semaphore2...\n");
    sem_wait(&semaphore2);
    printf("Thread 2: Acquired semaphore2, trying to acquire semaphore1...\n");

    // Simulate some processing
    sleep(2); // Increased sleep to ensure both threads reach deadlock
    
    sem_wait(&semaphore1);
    printf("Thread 2: Acquired semaphore1.\n");

    // Simulate some processing
    sleep(2);

    // Release the semaphores
    sem_post(&semaphore1);
    sem_post(&semaphore2);
    
    printf("Thread 2: Released semaphore1 and semaphore2.\n");

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Initialize semaphores
    sem_init(&semaphore1, 0, 1);
    sem_init(&semaphore2, 0, 1);

    // Create threads
    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);

    // Wait for threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy semaphores
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);

    return 0;
}


*/



#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function

// Shared semaphores
sem_t semaphore1;
sem_t semaphore2;

// Function executed by threads
void *thread_func1(void *arg) {
    printf("Thread 1 trying to acquire Semaphore 1...\n");
    sem_wait(&semaphore1); // Acquire Semaphore 1
    sleep(1); // Simulate some work

    printf("Thread 1 acquired Semaphore 1, trying to acquire Semaphore 2...\n");
    sem_wait(&semaphore2); // Attempt to acquire Semaphore 2

    // Critical section
    printf("Thread 1 in critical section\n");

    // Release semaphores
    sem_post(&semaphore2);
    sem_post(&semaphore1);
    
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 trying to acquire Semaphore 2...\n");
    sem_wait(&semaphore2); // Acquire Semaphore 2
    sleep(1); // Simulate some work

    printf("Thread 2 acquired Semaphore 2, trying to acquire Semaphore 1...\n");
    sem_wait(&semaphore1); // Attempt to acquire Semaphore 1

    // Critical section
    printf("Thread 2 in critical section\n");

    // Release semaphores
    sem_post(&semaphore1);
    sem_post(&semaphore2);
    
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Initialize semaphores
    sem_init(&semaphore1, 0, 1); // Semaphore 1 initialized to 1
    sem_init(&semaphore2, 0, 1); // Semaphore 2 initialized to 1

    // Create threads
    pthread_create(&t1, NULL, thread_func1, NULL);
    pthread_create(&t2, NULL, thread_func2, NULL);

    // Wait for threads to complete
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroy semaphores
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);

    return 0;
}



Thread 1 has acquired semaphore1 and is waiting to acquire semaphore2.
Thread 2 has acquired semaphore2 and is waiting to acquire semaphore1.


/*
/a.out
Thread 1 trying to acquire Semaphore 1...
Thread 2 trying to acquire Semaphore 2...
Thread 1 acquired Semaphore 1, trying to acquire Semaphore 2... 
Thread 2 acquired Semaphore 2, trying to acquire Semaphore 1...
//Thread 1 has acquired semaphore1 and is waiting to acquire semaphore2.
//Thread 2 has acquired semaphore2 and is waiting to acquire semaphore1.
*/


