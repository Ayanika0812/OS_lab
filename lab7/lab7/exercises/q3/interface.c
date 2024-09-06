// interface.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_com.h"

int main() {
    int shmid;
    struct shared_use_st *shared_stuff;
    void *shared_memory = (void *)0;
    char input[TEXT_SZ];

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
    shared_stuff->data_available = 0;

    while (1) {
        // Prompt for a message
        printf("Enter your message: ");
        if (fgets(input, TEXT_SZ, stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // Remove newline character from the input if present
        input[strcspn(input, "\n")] = 0;

        // Write to shared memory
        strncpy(shared_stuff->message, input, TEXT_SZ);
        shared_stuff->data_available = 1;

        // Wait for the Display to process the message
        while (shared_stuff->data_available == 1) {
            sleep(1);
        }
    }

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}


/*

gcc -o interface interface.c
 ./interface
Enter your message: hi
Enter your message: bye
Enter your message: ^C


gcc -o display display.c
 ./display
Received message: hi
Received message: bye
^C
 */

