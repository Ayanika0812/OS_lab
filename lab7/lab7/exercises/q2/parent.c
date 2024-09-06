// parent.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_com.h"

int main() {
    int shmid;
    struct shared_use_st *shared_stuff;
    void *shared_memory = (void *)0;
    char input;

    // Create shared memory segment
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shared_stuff = (struct shared_use_st *)shared_memory;

    // Initialize shared memory
    shared_stuff->written_by_parent = 0;

    // Get input from user
    printf("Enter an English alphabet: ");
    scanf(" %c", &input);

    // Write to shared memory
    shared_stuff->letter = input;
    shared_stuff->written_by_parent = 1;

    // Wait for child to respond
    while (shared_stuff->written_by_parent == 1) {
        sleep(1);
    }

    // Display the reply from child
    printf("Received from child: %c\n", shared_stuff->next_letter);

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Cleanup shared memory
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}

