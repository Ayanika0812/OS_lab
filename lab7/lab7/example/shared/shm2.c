// shm2.c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_com.h"

int main() {
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    char buffer[BUFSIZ];
    int shmid;

    // Create shared memory segment
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory segment
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %p\n", shared_memory);
    shared_stuff = (struct shared_use_st *)shared_memory;

    while (running) {
        while (shared_stuff->written_by_you == 1) {
            sleep(1);
            printf("waiting for client...\n");
        }

        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
        shared_stuff->written_by_you = 1;

        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}



/*

first run shm2(consumer

gcc -o shm2 shm2.c
~/Desktop/220905128/lab7/example/shared$ ./shm2
Memory attached at 0x782688144000
Enter some text: hello
waiting for client...
waiting for client...
waiting for client...
Enter some text: okay bye
waiting for client...
waiting for client...
Enter some text: end


gcc -o shm1 shm1.c
~/Desktop/220905128/lab7/example/shared$ ./shm1
Memory attached at 0x7a6d4e650000
You wrote: hello
You wrote: okay bye
You wrote: end


*/
