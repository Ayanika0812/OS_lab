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

    // Open the FIFO in read mode
    fd = open(FIFO_NAME, O_RDONLY);

    // Read 4 integers from the FIFO one at a time
    printf("Consumer: Reading integers from the FIFO queue:\n");
    for (int i = 0; i < 4; i++) {
        read(fd, &number, sizeof(number));  // Read an integer from the FIFO
        printf("Consumer: Read %d from the FIFO.\n", number);  // Print the integer
    }

    // Close the FIFO
    close(fd);

    return 0;
}