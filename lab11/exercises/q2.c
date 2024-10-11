#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS 10
#define MAX_RESOURCES 3

pthread_mutex_t lock;

int available[MAX_RESOURCES]; // Available resources
int maximum[MAX_THREADS][MAX_RESOURCES]; // Maximum resources needed by each thread
int allocation[MAX_THREADS][MAX_RESOURCES] = {0}; // Resources allocated to each thread
int need[MAX_THREADS][MAX_RESOURCES]; // Resources still needed by each thread

// Function to calculate the remaining need for each thread
void calculateNeed() {
    for (int i = 0; i < MAX_THREADS; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

// Function to check if the system is in a safe state
int isSafe() {
    int work[MAX_RESOURCES];
    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    int finish[MAX_THREADS] = {0};
    int count = 0;

    while (count < MAX_THREADS) {
        int found = 0;
        for (int p = 0; p < MAX_THREADS; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < MAX_RESOURCES; j++)
                    if (need[p][j] > work[j])
                        break;

                if (j == MAX_RESOURCES) {
                    for (int k = 0; k < MAX_RESOURCES; k++)
                        work[k] += allocation[p][k];

                    finish[p] = 1;
                    count++;
                    found = 1;
                }
            }
        }
        if (!found) {
            return 0; // Not in a safe state
        }
    }
    return 1; // Safe state
}

// Thread function to request resources
void *threadFunction(void *arg) {
    int threadID = *(int *)arg;
    free(arg); // Free the dynamically allocated memory

    int request[MAX_RESOURCES];

    // Get the resource request from the user
    printf("Thread %d: Enter resource request (e.g., 1 0 2) - max allowable: ", threadID);
    for (int i = 0; i < MAX_RESOURCES; i++) {
        printf("%d ", need[threadID][i]);
    }
    printf("\n");

    for (int i = 0; i < MAX_RESOURCES; i++) {
        scanf("%d", &request[i]);
    }

    pthread_mutex_lock(&lock);

    // Check if the request exceeds maximum claim or need
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > need[threadID][i]) {
            printf("Thread %d: Request exceeds maximum claim.\n", threadID);
            pthread_mutex_unlock(&lock);
            return NULL;
        }
        if (request[i] > available[i]) {
            printf("Thread %d: Resources not available. Waiting...\n", threadID);
            pthread_mutex_unlock(&lock);
            sleep(1); // Simulate waiting
            return NULL;
        }
    }

    // Pretend to allocate resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[threadID][i] += request[i];
        need[threadID][i] -= request[i];
    }

    // Check for safe state
    if (isSafe()) {
        printf("Thread %d: Request granted.\n", threadID);
    } else {
        printf("Thread %d: Request denied. Reverting changes.\n", threadID);
        for (int i = 0; i < MAX_RESOURCES; i++) {
            available[i] += request[i];
            allocation[threadID][i] -= request[i];
            need[threadID][i] += request[i];
        }
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];

    pthread_mutex_init(&lock, NULL);

    // Input number of threads
    int numThreads;
    printf("Enter number of threads (max %d): ", MAX_THREADS);
    scanf("%d", &numThreads);

    // Input available resources
    printf("Enter available resources (e.g., 5 0 0): ");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        scanf("%d", &available[i]);
    }

    // Input maximum resources for each thread
    for (int i = 0; i < numThreads; i++) {
        printf("Enter maximum resources for Thread %d (e.g., 5 0 0): ", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    calculateNeed();

    // Create threads
    for (int i = 0; i < numThreads; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&threads[i], NULL, threadFunction, arg);
    }

    // Join threads
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}

