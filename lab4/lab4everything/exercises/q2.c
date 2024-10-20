#include <unistd.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char ch, temp;
    int c = 0;

    for (int i = 1; i < argc; i++) {
        int in = open(argv[i], O_RDONLY);  // Corrected argv[i]
        if (in < 0) {
            perror("Error opening file");
            continue;  // Skip to the next file if opening fails
        }

        while (read(in, &ch, 1) == 1) {
            printf("%c", ch);  // Print each character as it is read
            if (ch == '\n') {
                c++;
            }

            if (c == 20) {  // If 20 lines have been read
                c = 0;
                printf("Press any key to continue...\n");
                scanf("%c", &temp);  // Wait for user input
            }
        }

        close(in);  // Close the file after reading
    }

    return 0;
}

/*
./a.out test3.txt
Line 1: Example line in file1
Line 2: Example line in file1
Line 3: Example line in file1
Line 4: Example line in file1
Line 5: Example line in file1
Line 6: Example line in file1
Line 7: Example line in file1
Line 8: Example line in file1
Line 9: Example line in file1
Line 10: Example line in file1
Line 11: Example line in file1
Line 12: Example line in file1
Line 13: Example line in file1
Line 14: Example line in file1
Line 15: Example line in file1
Line 16: Example line in file1
Line 17: Example line in file1
Line 18: Example line in file1
Line 19: Example line in file1
Line 20: Example line in file1
Press any key to continue...
k
Line 21: Example line in file1
Line 22: Example line in file1
Line 23: Example line in file1
*/
