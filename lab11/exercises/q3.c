#include <stdio.h>
#include <stdlib.h>

struct DSA {
    int request_id;
    int arrival_time_stamp;
    int cylinder;
    int address;
    int process_id;
};

// Function to compare two requests based on arrival time
int compareArrival(const void *a, const void *b) {
    return ((struct DSA *)a)->arrival_time_stamp - ((struct DSA *)b)->arrival_time_stamp;
}

// Function to find the shortest seek time request
int findSSTF(int head, struct DSA requests[], int num_requests, int processed[]) {
    int min_distance = __INT_MAX__;
    int index = -1;

    for (int i = 0; i < num_requests; i++) {
        if (!processed[i]) {
            int distance = abs(requests[i].cylinder - head);
            if (distance < min_distance) {
                min_distance = distance;
                index = i;
            }
        }
    }
    return index;
}

int main() {
    int num_requests;

    printf("Enter number of disk requests: ");
    scanf("%d", &num_requests);

    struct DSA requests[num_requests];

    // Input requests
    for (int i = 0; i < num_requests; i++) {
        printf("Enter details for request %d:\n", i + 1);
        printf("Request ID: ");
        scanf("%d", &requests[i].request_id);
        printf("Arrival Time Stamp: ");
        scanf("%d", &requests[i].arrival_time_stamp);
        printf("Cylinder: ");
        scanf("%d", &requests[i].cylinder);
        printf("Address: ");
        scanf("%d", &requests[i].address);
        printf("Process ID: ");
        scanf("%d", &requests[i].process_id);
    }

    // Sort requests based on arrival time for FCFS
    qsort(requests, num_requests, sizeof(struct DSA), compareArrival);

    printf("\n--- FCFS Disk Scheduling ---\n");
    for (int i = 0; i < num_requests; i++) {
        printf("Request ID: %d, Cylinder: %d, Arrival Time: %d, Process ID: %d\n", 
               requests[i].request_id, requests[i].cylinder, requests[i].arrival_time_stamp, requests[i].process_id);
    }

    // SSTF Scheduling
    printf("\n--- SSTF Disk Scheduling ---\n");
    int head;
    printf("Enter initial head position for SSTF: ");
    scanf("%d", &head);

    int processed[num_requests]; // To track processed requests
    for (int i = 0; i < num_requests; i++) {
        processed[i] = 0; // Initialize all as unprocessed
    }

    for (int count = 0; count < num_requests; count++) {
        int index = findSSTF(head, requests, num_requests, processed);
        if (index != -1) {
            printf("Processing Request ID: %d, Cylinder: %d, Arrival Time: %d, Process ID: %d\n", 
                   requests[index].request_id, requests[index].cylinder, requests[index].arrival_time_stamp, requests[index].process_id);
            head = requests[index].cylinder; // Move head to the cylinder of the processed request
            processed[index] = 1; // Mark as processed
        }
    }

    return 0;
}


/*
Enter number of disk requests: 3
Enter details for request 1:
Request ID: 1
Arrival Time Stamp: 3
Cylinder: 80
Address: 100
Process ID: 1
Enter details for request 2:
Request ID: 2
Arrival Time Stamp: 5
Cylinder: 100
Address: 200
Process ID: 2
Enter details for request 3:
Request ID: 3
Arrival Time Stamp: 0
Cylinder: 50
Address: 300
Process ID: 3

--- FCFS Disk Scheduling ---
Request ID: 3, Cylinder: 50, Arrival Time: 0, Process ID: 3
Request ID: 1, Cylinder: 80, Arrival Time: 3, Process ID: 1
Request ID: 2, Cylinder: 100, Arrival Time: 5, Process ID: 2

--- SSTF Disk Scheduling ---
Enter initial head position for SSTF: 95
Processing Request ID: 2, Cylinder: 100, Arrival Time: 5, Process ID: 2
Processing Request ID: 1, Cylinder: 80, Arrival Time: 3, Process ID: 1
Processing Request ID: 3, Cylinder: 50, Arrival Time: 0, Process ID: 3
*/

