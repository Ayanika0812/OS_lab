#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100
#define TIME_QUANTUM_1 3 // Time quantum for Queue 1
#define TIME_QUANTUM_2 6 // Time quantum for Queue 2

typedef struct Process {
    int id;
    int burst_time;
    int remaining_time;
    int arrival_time;
    struct Process* next;
} Process;

typedef struct Queue {
    Process* front;
    Process* rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, int id, int burst_time, int arrival_time) {
    Process* new_process = (Process*)malloc(sizeof(Process));
    new_process->id = id;
    new_process->burst_time = burst_time;
    new_process->remaining_time = burst_time;
    new_process->arrival_time = arrival_time;
    new_process->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = new_process;
        return;
    }
    q->rear->next = new_process;
    q->rear = new_process;
}

Process* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    Process* temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    return temp;
}

void freeQueue(Queue* q) {
    Process* current = q->front;
    while (current != NULL) {
        Process* temp = current;
        current = current->next;
        free(temp);
    }
    free(q);
}

void simulateMLFQ(Queue* queue1, Queue* queue2) {
    int time = 0;
    int processes_remaining = 0;

    // Count how many processes we have
    for (Process* p = queue1->front; p != NULL; p = p->next) {
        processes_remaining++;
    }

    while (processes_remaining > 0) {
        // Process Queue 1
        if (queue1->front) {
            Process* p = dequeue(queue1);
            if (p->arrival_time > time) {
                // Process hasn't arrived yet; push it back to the queue
                enqueue(queue1, p->id, p->burst_time, p->arrival_time);
                time++;
                continue;
            }

            int time_slice = p->remaining_time < TIME_QUANTUM_1 ? p->remaining_time : TIME_QUANTUM_1;
            time += time_slice;
            p->remaining_time -= time_slice;

            if (p->remaining_time == 0) {
                printf("Process %d finished at time %d\n", p->id, time);
                free(p);
                processes_remaining--;
            } else {
                enqueue(queue2, p->id, p->remaining_time, time);
                free(p);
            }
        } else {
            time++;
        }

        // Process Queue 2
        if (queue2->front) {
            Process* p = dequeue(queue2);
            if (p->arrival_time > time) {
                // Process hasn't arrived yet; push it back to the queue
                enqueue(queue2, p->id, p->burst_time, p->arrival_time);
                time++;
                continue;
            }

            int time_slice = p->remaining_time < TIME_QUANTUM_2 ? p->remaining_time : TIME_QUANTUM_2;
            time += time_slice;
            p->remaining_time -= time_slice;

            if (p->remaining_time == 0) {
                printf("Process %d finished at time %d\n", p->id, time);
                free(p);
                processes_remaining--;
            } else {
                enqueue(queue1, p->id, p->remaining_time, time);
                free(p);
            }
        }
    }
}

int main() {
    Queue* queue1 = createQueue();
    Queue* queue2 = createQueue();

    int num_processes;
    printf("FIRSE COME FIRST SERVE \n");
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    for (int i = 0; i < num_processes; i++) {
        int burst_time, arrival_time;
        printf("Enter burst time and arrival time for process %d: ", i + 1);
        scanf("%d %d", &burst_time, &arrival_time);
        enqueue(queue1, i + 1, burst_time, arrival_time);
    }

    simulateMLFQ(queue1, queue2);

    freeQueue(queue1);
    freeQueue(queue2);

    return 0;
}


/*

Enter number of processes: 3
Enter burst time and arrival time for process 1: 3 1
Enter burst time and arrival time for process 2: 4 2
Enter burst time and arrival time for process 3: 2 0
Process 3 finished at time 4
Process 1 finished at time 7
Process 2 finished at time 11


*/
