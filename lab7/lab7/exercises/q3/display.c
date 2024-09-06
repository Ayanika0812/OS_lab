// display.c
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

    while (1) {
        // Wait until data is available
        while (shared_stuff->data_available == 0) {
            sleep(1);
        }

        // Read and print the message
        printf("Received message: %s\n", shared_stuff->message);

        // Clear the shared memory
        shared_stuff->data_available = 0;
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

