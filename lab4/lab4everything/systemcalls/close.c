#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
char c;
int in, out;
in = open ("file1.in", O_RDONLY);
out = open("file1.out", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
// printf("hello inside close");
// printf("hello out close");
while(read(in,&c,1) == 1)
write(out,&c,1);
exit(0);
}