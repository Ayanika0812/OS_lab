// process_a.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg_queue.h"

int main() {
    int msgid;
    struct msg_buffer msg;
    
    // Create message queue
    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Read number from user
    printf("Enter a number: ");
    scanf("%d", &msg.number);

    // Set message type and send the number
    msg.msg_type = MSG_TYPE;
    if (msgsnd(msgid, &msg, sizeof(msg.number), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Number sent to Process B.\n");

    return 0;
}


/*

gcc -o a PA.c
./a
Enter a number: 1221
Number sent to Process B.
./a
Enter a number: 121
Number sent to Process B.
 ./a
Enter a number: 1234
Number sent to Process B.


gcc -o b PB.c
 ./b
msgget: No such file or directory
 ./b
The number 1221 is a palindrome.
 ./b
The number 121 is a palindrome.
 ./b
The number 1234 is not a palindrome.
 gcc -o b PB.c
 ./b
msgget: No such file or directory
Message queue does not exist. Ensure that Process A is run first.


