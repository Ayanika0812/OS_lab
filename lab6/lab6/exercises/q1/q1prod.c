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

    mkfifo(FIFO_NAME, 0777);

    // Open in write mode
    fd = open(FIFO_NAME, O_WRONLY);
    
    for (int i = 0; i < 4; i++) {
        printf("Producer: Enter an integer: ");
        scanf("%d", &number);  
        write(fd, &number, sizeof(number));  
        printf("Producer: Wrote %d to the FIFO.\n", number);  
    }

    printf("Producer: Finished writing integers in FIFO queue.\n");

    close(fd);

    return 0;
}

/* output -

first compile and run producer then go for consumer

/a.out
Producer: Enter an integer: 2
Producer: Wrote 2 to the FIFO.
Producer: Enter an integer: 3
Producer: Wrote 3 to the FIFO.
Producer: Enter an integer: 5
Producer: Wrote 5 to the FIFO.
Producer: Enter an integer: 6
Producer: Wrote 6 to the FIFO.
Producer: Finished writing integers in FIFO queue.

*/
