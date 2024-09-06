// process_b.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "msg_queue.h"

int is_palindrome(int num) {
    char str[12];
    char reversed[12];
    int len, i;

    // Convert number to string
    snprintf(str, sizeof(str), "%d", num);

    // Reverse the string
    len = strlen(str);
    for (i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';

    // Check if original and reversed strings are the same
    return strcmp(str, reversed) == 0;
}

int main() {
    int msgid;
    struct msg_buffer msg;

    // Create or access the message queue
    // Access the message queue
msgid = msgget(MSG_KEY, 0666);
if (msgid == -1) {
    perror("msgget");
    printf("Message queue does not exist. Ensure that Process A is run first.\n");
    exit(EXIT_FAILURE);
}


    // Receive message
    if (msgrcv(msgid, &msg, sizeof(msg.number), MSG_TYPE, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    // Check if number is a palindrome
    if (is_palindrome(msg.number)) {
        printf("The number %d is a palindrome.\n", msg.number);
    } else {
        printf("The number %d is not a palindrome.\n", msg.number);
    }

    // Remove message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}

