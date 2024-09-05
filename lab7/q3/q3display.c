#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char *)shmat(shmid, (void *)0, 0);

    while (1) {
        if (strlen(str) > 0) {
            printf("Received message: %s", str);
            memset(str, 0, 1024);  // Clear the shared memory
        }
    }

    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
