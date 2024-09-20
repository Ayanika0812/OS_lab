#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

typedef struct Process {
    int id;
    int burst_time;
    int remaining_time;
    int arrival_time;
} Process;

void simulateRoundRobin(Process processes[], int num_processes, int time_quantum) {
    int time = 0;
    int completed = 0;

    while (completed < num_processes) {
        int all_waited = 1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                all_waited = 0;

                int time_slice = (processes[i].remaining_time < time_quantum) ? processes[i].remaining_time : time_quantum;
                time += time_slice;
                processes[i].remaining_time -= time_slice;

                if (processes[i].remaining_time == 0) {
                    printf("Process %d finished at time %d\n", processes[i].id, time);
                    completed++;
                }
            }
        }

        // If all processes are waiting (not arrived yet), increment time
        if (all_waited) {
            time++;
        }
    }
}

int main() {
    Process processes[MAX_PROCESSES];
    int num_processes;
    int time_quantum;

    printf("Enter number of processes: ");
    scanf("%d", &num_processes);
    
    for (int i = 0; i < num_processes; i++) {
        printf("Enter burst time and arrival time for process %d: ", i + 1);
        scanf("%d %d", &processes[i].burst_time, &processes[i].arrival_time);
        processes[i].id = i + 1;
        processes[i].remaining_time = processes[i].burst_time;
    }

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    simulateRoundRobin(processes, num_processes, time_quantum);

    return 0;
}

/*

Enter number of processes: 3
Enter burst time and arrival time for process 1: 4 1
Enter burst time and arrival time for process 2: 3 0
Enter burst time and arrival time for process 3: 5 2
Enter time quantum: 2
Process 2 finished at time 7
Process 1 finished at time 11
Process 3 finished at time 12
*/

