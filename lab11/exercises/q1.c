#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 3

int allocation[MAX_PROCESSES][MAX_RESOURCES]; // Resources allocated to each process
int maximum[MAX_PROCESSES][MAX_RESOURCES]; // Maximum resources needed by each process
int available[MAX_RESOURCES]; // Available resources
int need[MAX_PROCESSES][MAX_RESOURCES]; // Resources still needed by each process

// Function to calculate the Need matrix
void calculateNeed(int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

// Function to check if the system is in a safe state
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
                if (j == MAX_RESOURCES) { // All resources can be allocated to process p
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
            return false; // Not in a safe state
        }
    }

    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < count; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");
    return true; // Safe state
}

// Function to check if a request can be granted
bool requestResources(int processID, int request[]) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > need[processID][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
        if (request[i] > available[i]) {
            printf("Resources not available. Process must wait.\n");
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
    return isSafe(MAX_PROCESSES);
}

// Main function
int main() {
    int numProcesses;

    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);

    printf("Enter the available resources (A, B, C): ");
    scanf("%d %d %d", &available[0], &available[1], &available[2]);

    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("Enter the maximum matrix:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    // Calculate Need matrix
    calculateNeed(numProcesses);

    // Check if the system is in a safe state
    isSafe(numProcesses);

    // Request from P1 for (1, 0, 2)
    int request1[3] = {1, 0, 2};
    printf("\nRequest from P1 for (1, 0, 2):\n");
    if (requestResources(1, request1)) {
        printf("Request granted.\n");
    } else {
        printf("Request denied.\n");
    }

    // Request from P4 for (3, 3, 0)
    int request4[3] = {3, 3, 0};
    printf("\nRequest from P4 for (3, 3, 0):\n");
    if (requestResources(4, request4)) {
        printf("Request granted.\n");
    } else {
        printf("Request denied.\n");
    }

    // Request from P0 for (0, 2, 0)
    int request0[3] = {0, 2, 0};
    printf("\nRequest from P0 for (0, 2, 0):\n");
    if (requestResources(0, request0)) {
        printf("Request granted.\n");
    } else {
        printf("Request denied.\n");
    }

    return 0;
}
/*

./a.out
Enter the number of processes: 5
Enter the available resources (A, B, C): 3 3 2
Enter the allocation matrix:
Process P0: 0 1 0
Process P1: 2 0 0
Process P2: 3 0 2
Process P3: 2 1 1
Process P4: 0 0 2
Enter the maximum matrix:
Process P0: 7 5 3
Process P1: 3 2 2
Process P2: 9 0 2
Process P3: 2 2 2
Process P4: 4 3 3
System is in a safe state.
Safe sequence is: P1 P3 P4 P0 P2 

Request from P1 for (1, 0, 2):
System is in a safe state.
Safe sequence is: P1 P3 P4 P5 P6 P7 P8 P9 P0 P2 
Request granted.

Request from P4 for (3, 3, 0):
Resources not available. Process must wait.
Request denied.

Request from P0 for (0, 2, 0):
System is not in a safe state.
Request denied.
*/
