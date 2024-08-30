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

    // Open in read mode
    fd = open(FIFO_NAME, O_RDONLY);

    printf("Consumer: Reading integers from the FIFO queue:\n");
    for (int i = 0; i < 4; i++) {
        read(fd, &number, sizeof(number)); 
        printf("Consumer: Read %d from the FIFO.\n", number); 
    }

    close(fd);

    return 0;
}

/* output :

./a.out
Consumer: Reading integers from the FIFO queue:
Consumer: Read 8 from the FIFO.
Consumer: Read 7 from the FIFO.
Consumer: Read 9 from the FIFO.
Consumer: Read 5 from the FIFO.
 */
