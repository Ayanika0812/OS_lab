#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 3

int allocation[MAX_PROCESSES][MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

pthread_mutex_t lock;
pthread_cond_t cond;
int waitingProcesses = 0;

void calculateNeed(int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

bool isSafe(int numProcesses) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safeSeq[MAX_PROCESSES];
    int count = 0;

    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    while (count < numProcesses) {
        bool found = false;
        for (int p = 0; p < numProcesses; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < MAX_RESOURCES; j++) {
                    if (need[p][j] > work[j]) break;
                }
                if (j == MAX_RESOURCES) {
                    for (int k = 0; k < MAX_RESOURCES; k++) {
                        work[k] += allocation[p][k];
                    }
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            printf("System is not in a safe state.\n");
            return false;
        }
    }

    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < count; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");
    return true;
}

bool requestResources(int processID, int request[]) {
    pthread_mutex_lock(&lock);

    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > need[processID][i]) {
            printf("Error: Process P%d has exceeded its maximum claim.\n", processID);
            pthread_mutex_unlock(&lock);
            return false;
        }
        if (request[i] > available[i]) {
            printf("Resources not available. Process P%d must wait.\n", processID);
            waitingProcesses++;
            pthread_cond_wait(&cond, &lock);
            waitingProcesses--;
            pthread_mutex_unlock(&lock);
            return false;
        }
    }

    // Pretend to allocate resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[processID][i] += request[i];
        need[processID][i] -= request[i];
    }

    // Check for safe state
    bool safe = isSafe(MAX_PROCESSES);

    if (!safe) {
        // Rollback if not safe
        for (int i = 0; i < MAX_RESOURCES; i++) {
            available[i] += request[i];
            allocation[processID][i] -= request[i];
            need[processID][i] += request[i];
        }
    } else {
        if (waitingProcesses > 0) {
            pthread_cond_broadcast(&cond);
        }
    }

    pthread_mutex_unlock(&lock);
    return safe;
}

void releaseResources(int processID, int release[]) {
    pthread_mutex_lock(&lock);

    for (int i = 0; i < MAX_RESOURCES; i++) {
        allocation[processID][i] -= release[i];
        available[i] += release[i];
    }

    pthread_mutex_unlock(&lock);
}

void* processThread(void* arg) {
    int processID = *(int*)arg;

    // Request resources interactively
    int request[MAX_RESOURCES];
    printf("Process P%d, enter resource request (A, B, C): ", processID);
    scanf("%d %d %d", &request[0], &request[1], &request[2]);

    printf("Process P%d requesting resources: (%d, %d, %d)\n", processID, request[0], request[1], request[2]);
    if (requestResources(processID, request)) {
        printf("Process P%d request granted.\n", processID);
        sleep(1); // Simulate processing
        releaseResources(processID, request);
        printf("Process P%d released resources.\n", processID);
    } else {
        printf("Process P%d request denied.\n", processID);
    }

    return NULL;
}

int main() {
    pthread_t threads[MAX_PROCESSES];
    int processIDs[MAX_PROCESSES];
    int numProcesses;

    printf("Enter the number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &numProcesses);

    printf("Enter the available resources (A, B, C): ");
    scanf("%d %d %d", &available[0], &available[1], &available[2]);

    // Input allocation matrix
    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Input maximum matrix
    printf("Enter the maximum matrix:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    // Calculate Need matrix and check initial safe state
    calculateNeed(numProcesses);
    isSafe(numProcesses);  // Print the initial safe state

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // Create threads for each process
    for (int i = 0; i < numProcesses; i++) {
        processIDs[i] = i;
        pthread_create(&threads[i], NULL, processThread, &processIDs[i]);
    }

    // Join threads
    for (int i = 0; i < numProcesses; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}

