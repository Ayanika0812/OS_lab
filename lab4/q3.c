/*Demonstrate the use of different conversion specifiers and resulting output to allow the items to be printed.*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[256];

int main() {
    char *msg1 = "Integer: ";
    char *msg2 = "Float: ";
    char *msg3 = "String: ";
    char *msg4 = "Character: ";

    int i = 123;
    float f = 45.67;
    char *s = "Hello, World!";
    char c = 'A';
    int len;
    
    // The snprintf function itself does not print anything, it formats a string and stores it in a buffer.
    len = snprintf(buffer, sizeof(buffer), "%s%d\n", msg1, i);
    //printf("hello %s", buffer);
    write(1, buffer, len);

    len = snprintf(buffer, sizeof(buffer), "%s%.2f\n", msg2, f);
    write(1, buffer, len);

    len = snprintf(buffer, sizeof(buffer), "%s%s\n", msg3, s);
    write(1, buffer, len);

    len = snprintf(buffer, sizeof(buffer), "%s%c\n", msg4, c);
    write(1, buffer, len);

    return 0;
}

