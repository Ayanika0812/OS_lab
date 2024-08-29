#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;
    int number;

    // Create the FIFO if it doesn't exist
    mkfifo(FIFO_NAME, 0666);

    // Open the FIFO in write mode
    fd = open(FIFO_NAME, O_WRONLY);

    // Write 4 integers to the FIFO one at a time
    for (int i = 0; i < 4; i++) {
        printf("Producer: Enter an integer: ");
        scanf("%d", &number);  // Read an integer from the user
        write(fd, &number, sizeof(number));  // Write the integer to the FIFO
        printf("Producer: Wrote %d to the FIFO.\n", number);  // Confirm the write
    }

    printf("Producer: Finished writing integers to the FIFO queue.\n");

    // Close the FIFO
    close(fd);

    return 0;
}
