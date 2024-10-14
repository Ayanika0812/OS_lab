#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_THREADS 10
#define MAX_RESOURCES 5

int n, m; // n: Number of processes (threads), m: Number of resource types
int Allocation[MAX_THREADS][MAX_RESOURCES], Max[MAX_THREADS][MAX_RESOURCES], Need[MAX_THREADS][MAX_RESOURCES], Available[MAX_RESOURCES];

// Mutex lock for shared data
pthread_mutex_t lock;

bool isSafe() {
    int Work[MAX_RESOURCES], Finish[MAX_THREADS];
    for (int i = 0; i < m; i++) {
        Work[i] = Available[i];
    }
    for (int i = 0; i < n; i++) {
        Finish[i] = 0;
    }

    int safeSequence[MAX_THREADS];
    int count = 0;

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (Finish[i] == 0) {
                int j;
                for (j = 0; j < m; j++) {
                    if (Need[i][j] > Work[j]) {
                        break;
                    }
                }

                if (j == m) {  // If all needs can be satisfied
                    for (int k = 0; k < m; k++) {
                        Work[k] += Allocation[i][k];
                    }
                    safeSequence[count++] = i;
                    Finish[i] = 1;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("System is not in a safe state.\n");
            return false;
        }
    }

    printf("System is in a safe state.\nSafe sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", safeSequence[i]);
    }
    printf("\n");
    return true;
}

void *process(void *arg) {
    int p = *(int *)arg;
    pthread_mutex_lock(&lock);

    printf("\nProcess %d is requesting resources.\n", p);
    int Request[MAX_RESOURCES];
    printf("Enter request for Process %d: ", p);
    for (int i = 0; i < m; i++) {
        scanf("%d", &Request[i]);
    }

    bool canRequest = true;
    for (int i = 0; i < m; i++) {
        if (Request[i] > Need[p][i]) {
            canRequest = false;
            break;
        }
    }

    if (canRequest) {
        for (int i = 0; i < m; i++) {
            if (Request[i] > Available[i]) {
                printf("Not enough resources available. Process %d must wait.\n", p);
                pthread_mutex_unlock(&lock);
                return NULL;
            }
        }

        // Temporarily allocate the resources to the process
        for (int i = 0; i < m; i++) {
            Available[i] -= Request[i];
            Allocation[p][i] += Request[i];
            Need[p][i] -= Request[i];
        }

        if (isSafe()) {
            printf("Request by Process %d is granted.\n", p);
        } else {
            printf("Request by Process %d cannot be granted (unsafe state). Rolling back.\n", p);
            // Rollback
            for (int i = 0; i < m; i++) {
                Available[i] += Request[i];
                Allocation[p][i] -= Request[i];
                Need[p][i] += Request[i];
            }
        }
    } else {
        printf("Request by Process %d exceeds its maximum needs.\n", p);
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    // Initialize the mutex lock
    pthread_mutex_init(&lock, NULL);

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    // Input Allocation Matrix
    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &Allocation[i][j]);
        }
    }

    // Input Max Matrix
    printf("Enter Max Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &Max[i][j]);
            Need[i][j] = Max[i][j] - Allocation[i][j]; // Calculate Need
        }
    }

    // Input Available Resources
    printf("Enter Available Resources: ");
    for (int i = 0; i < m; i++) {
        scanf("%d", &Available[i]);
    }

    pthread_t tid[MAX_THREADS];
    int process_ids[MAX_THREADS];

    // Create threads for each process
    for (int i = 0; i < n; i++) {
        process_ids[i] = i;
        pthread_create(&tid[i], NULL, process, &process_ids[i]);
    }

    // Join threads
    for (int i = 0; i < n; i++) {
        pthread_join(tid[i], NULL);
    }

    // Destroy the mutex lock
    pthread_mutex_destroy(&lock);

    return 0;
}
/*

./a.out
Enter number of processes: 5
Enter number of resource types: 3
Enter Allocation Matrix:
Process 0: 0 1 0
Process 1: 2 0 0
Process 2: 3 0 2
Process 3: 2 1 1
Process 4: 0 0 2
Enter Max Matrix:
Process 0: 7 5 3
Process 1: 3 2 2
Process 2: 9 0 2
Process 3: 2 2 2
Process 4: 4 3 3
Enter Available Resources: 3 3 2

Process 0 is requesting resources.
Enter request for Process 0: 0 0 0
System is in a safe state.
Safe sequence: 1 3 4 0 2 
Request by Process 0 is granted.

Process 1 is requesting resources.
Enter request for Process 1: 1 0 2
System is in a safe state.
Safe sequence: 1 3 4 0 2 
Request by Process 1 is granted.

Process 2 is requesting resources.
Enter request for Process 2: 0 0 0 
System is in a safe state.
Safe sequence: 1 3 4 0 2 
Request by Process 2 is granted.

Process 4 is requesting resources.
Enter request for Process 4: 3 3 0
Not enough resources available. Process 4 must wait.

Process 3 is requesting resources.
Enter request for Process 3: 0 0 0
System is in a safe state.
Safe sequence: 1 3 4 0 2 
Request by Process 3 is granted.

*/
