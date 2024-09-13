/*#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function
#include <stdlib.h> // For rand()

#define CHAIRS 5 // Number of waiting chairs

// Semaphores and shared variables
sem_t barberReady;  // Semaphore for the barber to wait for customers
sem_t customerReady; // Semaphore for customers to wait for the barber
pthread_mutex_t mutex; // Mutex to protect shared variables
int waitingCustomers = 0; // Number of waiting customers

void* barber(void* arg) {
    while (1) {
        // Wait for customers
        sem_wait(&customerReady);
        pthread_mutex_lock(&mutex);
        
        // Customer is ready, so reduce waiting count
        waitingCustomers--;
        printf("Barber: Cutting hair. Waiting customers: %d\n", waitingCustomers);
        
        pthread_mutex_unlock(&mutex);
        sleep(2); // Simulate time taken to cut hair
        
        // Signal the customer that their haircut is done
        sem_post(&barberReady);
    }
    return NULL;
}

void* customer(void* arg) {
    int id = *((int*)arg);
    free(arg);
    
    pthread_mutex_lock(&mutex);
    if (waitingCustomers < CHAIRS) {
        // There is a free chair
        waitingCustomers++;
        printf("Customer %d: Sitting in the waiting room. Waiting customers: %d\n", id, waitingCustomers);
        
        // Signal the barber that a customer is ready
        sem_post(&customerReady);
        pthread_mutex_unlock(&mutex);
        
        // Wait for the barber to be ready
        sem_wait(&barberReady);
        printf("Customer %d: Getting a haircut.\n", id);
    } else {
        // No free chair, customer leaves
        pthread_mutex_unlock(&mutex);
        printf("Customer %d: No free chair, leaving.\n", id);
    }
    return NULL;
}

int main() {
    pthread_t barberThread;
    pthread_t customerThreads[20]; // Array for customer threads
    int i;

    // Initialize semaphores and mutex
    sem_init(&barberReady, 0, 0);
    sem_init(&customerReady, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create barber thread
    pthread_create(&barberThread, NULL, barber, NULL);

    // Create customer threads
    for (i = 0; i < 20; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&customerThreads[i], NULL, customer, id);
        sleep(rand() % 3); // Random delay between customer arrivals
    }

    // Wait for all customer threads to finish
    for (i = 0; i < 20; i++) {
        pthread_join(customerThreads[i], NULL);
    }

    // Cleanup
    pthread_cancel(barberThread); // In a real application, we should handle thread cancellation more gracefully
    pthread_join(barberThread, NULL);

    sem_destroy(&barberReady);
    sem_destroy(&customerReady);
    pthread_mutex_destroy(&mutex);

    return 0;
}


/*  
 ./q4
Customer 1: Sitting in the waiting room. Waiting customers: 1
Barber: Cutting hair. Waiting customers: 0
Customer 2: Sitting in the waiting room. Waiting customers: 1
Barber: Cutting hair. Waiting customers: 0
Customer 1: Getting a haircut.
Customer 3: Sitting in the waiting room. Waiting customers: 1
Customer 4: Sitting in the waiting room. Waiting customers: 2
Customer 5: Sitting in the waiting room. Waiting customers: 3
Barber: Cutting hair. Waiting customers: 2
Customer 2: Getting a haircut.
Customer 6: Sitting in the waiting room. Waiting customers: 3
Barber: Cutting hair. Waiting customers: 2
Customer 3: Getting a haircut.
Customer 7: Sitting in the waiting room. Waiting customers: 3
Customer 8: Sitting in the waiting room. Waiting customers: 4
Customer 9: Sitting in the waiting room. Waiting customers: 5
Customer 10: No free chair, leaving.
Barber: Cutting hair. Waiting customers: 4
Customer 4: Getting a haircut.
Customer 11: Sitting in the waiting room. Waiting customers: 5
Barber: Cutting hair. Waiting customers: 4
Customer 5: Getting a haircut.
Customer 12: Sitting in the waiting room. Waiting customers: 5
Customer 13: No free chair, leaving.
Barber: Cutting hair. Waiting customers: 4
Customer 6: Getting a haircut.
Customer 14: Sitting in the waiting room. Waiting customers: 5
Barber: Cutting hair. Waiting customers: 4
Customer 7: Getting a haircut.
Customer 15: Sitting in the waiting room. Waiting customers: 5
Barber: Cutting hair. Waiting customers: 4
Customer 8: Getting a haircut.
Customer 16: Sitting in the waiting room. Waiting customers: 5
Customer 17: No free chair, leaving.
Customer 18: No free chair, leaving.
Customer 19: No free chair, leaving.
Barber: Cutting hair. Waiting customers: 4
Customer 9: Getting a haircut.
Customer 20: Sitting in the waiting room. Waiting customers: 5
Barber: Cutting hair. Waiting customers: 4
Customer 11: Getting a haircut.
Barber: Cutting hair. Waiting customers: 3
Customer 12: Getting a haircut.
Barber: Cutting hair. Waiting customers: 2
Customer 14: Getting a haircut.
Barber: Cutting hair. Waiting customers: 1
Customer 15: Getting a haircut.
Barber: Cutting hair. Waiting customers: 0
Customer 16: Getting a haircut.
Customer 20: Getting a haircut.
*/




#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h> // For sleep function

#define NUM_CHAIRS 3 // Number of chairs in the waiting room

// Semaphores and shared variables
sem_t waiting_room; // Semaphore for managing the waiting room chairs
sem_t barber;       // Semaphore for the barber to sleep/wake up
sem_t customer;     // Semaphore for customers to indicate they are ready
int waiting_customers = 0; // Number of waiting customers

// Function executed by the barber thread
void *barber_func(void *arg) {
    while (1) {
        // Wait for a customer to arrive
        sem_wait(&customer);
        
        // Customer has arrived, so wake up
        sem_wait(&waiting_room);
        
        // Cut hair
        printf("Barber is cutting hair\n");
        sleep(2); // Simulate haircut duration
        
        // Done cutting hair
        sem_post(&waiting_room);
        
        // Signal customer that the haircut is done
        sem_post(&barber);
    }
    return NULL;
}

// Function executed by the customer threads
void *customer_func(void *arg) {
    // Try to acquire a chair in the waiting room
    if (sem_trywait(&waiting_room) == 0) {
        // Successfully acquired a chair
        printf("Customer %d is waiting\n", *((int *)arg));
        
        // Notify barber that there is a customer
        sem_post(&customer);
        
        // Wait for the barber to finish cutting hair
        sem_wait(&barber);
        
        // Done with haircut
        printf("Customer %d is done with the haircut\n", *((int *)arg));
    } else {
        // No chairs available
        printf("Customer %d found no available chairs and leaves\n", *((int *)arg));
    }
    return NULL;
}

int main() {
    pthread_t barber_thread;
    pthread_t customer_threads[10];
    int customer_ids[10];
    
    // Initialize semaphores
    sem_init(&waiting_room, 0, NUM_CHAIRS); // Initialize with number of chairs
    sem_init(&barber, 0, 0); // Initialize to 0, barber starts asleep
    sem_init(&customer, 0, 0); // Initialize to 0, no customer initially
    
    // Create barber thread
    pthread_create(&barber_thread, NULL, barber_func, NULL);
    
    // Create customer threads
    for (int i = 0; i < 10; i++) {
        customer_ids[i] = i + 1; // Customer IDs from 1 to 10
        pthread_create(&customer_threads[i], NULL, customer_func, &customer_ids[i]);
        sleep(1); // Simulate time between customer arrivals
    }
    
    // Wait for all customer threads to finish
    for (int i = 0; i < 10; i++) {
        pthread_join(customer_threads[i], NULL);
    }
    
    // Terminate barber thread (in practice, we might need a more graceful shutdown)
    pthread_cancel(barber_thread);
    
    // Destroy semaphores
    sem_destroy(&waiting_room);
    sem_destroy(&barber);
    sem_destroy(&customer);
    
    return 0;
}
/*

./a.out
Customer 1 is waiting
Barber is cutting hair
Customer 2 is waiting
Barber is cutting hair
Customer 1 is done with the haircut
Customer 3 found no available chairs and leaves
Customer 4 found no available chairs and leaves
Customer 2 is done with the haircut
Customer 5 is waiting
Customer 6 found no available chairs and leaves
Customer 7 found no available chairs and leaves
Customer 8 found no available chairs and leaves
Customer 9 found no available chairs and leaves
Customer 10 found no available chairs and leaves

*/

