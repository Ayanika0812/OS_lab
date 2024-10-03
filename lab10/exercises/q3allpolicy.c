#include <stdio.h>
#include <stdlib.h>

#define TIME_QUANTUM_1 3

typedef struct Process {
    int id, burst_time, remaining_time, arrival_time;
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

void enqueue(Queue* q, Process* p) {
    p->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = p;
        return;
    }
    q->rear->next = p;
    q->rear = p;
}

Process* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    Process* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    return temp;
}

void addProcess(Queue* q, int id, int burst_time, int arrival_time) {
    Process* p = (Process*)malloc(sizeof(Process));
    p->id = id; p->burst_time = burst_time;
    p->remaining_time = burst_time; p->arrival_time = arrival_time;
    enqueue(q, p);
}

Process* getShortestJob(Queue* q) {
    Process *shortest = q->front, *prev = NULL, *cur = q->front, *prev_min = NULL;
    while (cur != NULL) {
        if (cur->remaining_time < shortest->remaining_time) {
            shortest = cur;
            prev_min = prev;
        }
        prev = cur;
        cur = cur->next;
    }
    if (prev_min != NULL) prev_min->next = shortest->next;
    else q->front = shortest->next;
    if (shortest == q->rear) q->rear = prev_min;
    return shortest;
}

void simulateMLFQ(Queue* q1, Queue* q2, Queue* q3) {
    int time = 0;
    while (q1->front || q2->front || q3->front) {
        Process* p = NULL;
        
        // Queue 1: Round Robin
        if ((p = dequeue(q1))) {
            int slice = (p->remaining_time < TIME_QUANTUM_1) ? p->remaining_time : TIME_QUANTUM_1;
            time += slice; p->remaining_time -= slice;
            if (p->remaining_time > 0) enqueue(q2, p);
            else { printf("Process %d finished at time %d\n", p->id, time); free(p); }
            continue;
        }

        // Queue 2: Shortest Job First
        if (q2->front && (p = getShortestJob(q2))) {
            time += p->remaining_time; p->remaining_time = 0;
            printf("Process %d finished at time %d\n", p->id, time); free(p);
            continue;
        }

        // Queue 3: First Come First Serve
        if ((p = dequeue(q3))) {
            time += p->remaining_time; p->remaining_time = 0;
            printf("Process %d finished at time %d\n", p->id, time); free(p);
            continue;
        }

        time++;
    }
}

int main() {
    Queue *q1 = createQueue(), *q2 = createQueue(), *q3 = createQueue();
    int num_processes;

    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    for (int i = 0; i < num_processes; i++) {
        int burst_time, arrival_time;
        printf("Enter burst time and arrival time for process %d: ", i + 1);
        scanf("%d %d", &burst_time, &arrival_time);
        addProcess(q1, i + 1, burst_time, arrival_time);
    }

    simulateMLFQ(q1, q2, q3);
    free(q1); free(q2); free(q3);
    return 0;
}
/*./a.out
Enter number of processes: 3
Enter burst time and arrival time for process 1: 3 1
Enter burst time and arrival time for process 2: 4 2
Enter burst time and arrival time for process 3: 2 0
Process 1 finished at time 3
Process 3 finished at time 8
Process 2 finished at time 9
*/
