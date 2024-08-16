#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    /* code */

    int fd = open("draft1.txt", O_RDONLY | O_CREAT);
    printf("fd = %d\n", fd);
    if(fd == -1) {
        printf("Error");
    }
    char c[100];

    int n = read(fd, c, 100);
    printf("n = %d\n",n);
    write(1, c, n);


    return 0;
}

