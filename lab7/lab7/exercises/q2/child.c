// child.c
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

    // Access the shared memory segment
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666);
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

    // Wait for parent to send data
    while (shared_stuff->written_by_parent == 0) {
        sleep(1);
    }

    // Compute the next letter
    if (shared_stuff->letter >= 'A' && shared_stuff->letter < 'Z') {
        shared_stuff->next_letter = shared_stuff->letter + 1;
    } else if (shared_stuff->letter == 'Z') {
        shared_stuff->next_letter = 'A';  // Wrap around from Z to A
    } else if (shared_stuff->letter >= 'a' && shared_stuff->letter < 'z') {
        shared_stuff->next_letter = shared_stuff->letter + 1;
    } else if (shared_stuff->letter == 'z') {
        shared_stuff->next_letter = 'a';  // Wrap around from z to a
    } else {
        shared_stuff->next_letter = shared_stuff->letter;  // Non-alphabetic input
    }

    // Signal that child has written the next letter
    shared_stuff->written_by_parent = 0;

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}

