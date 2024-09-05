#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX 100

struct message {
    long msg_type;
    char msg_text[MAX];
};

int isPalindrome(int num) {
    int reversed = 0, original = num, remainder;
    while (num != 0) {
        remainder = num % 10;
        reversed = reversed * 10 + remainder;
        num /= 10;
    }
    return original == reversed;
}

int main() {
    struct message msg;
    key_t key;
    int msgid;

    // Generate a unique key
    key = ftok("progfile", 65);

    // Create a message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Process A: Sending number
    printf("Process A: Enter a number: ");
    fgets(msg.msg_text, MAX, stdin);
    msg.msg_type = 1;

    // Send the message
    msgsnd(msgid, &msg, sizeof(msg), 0);
    printf("Process A: Number sent to Process B.\n");

    // Process B: Receiving and checking palindrome
    msgrcv(msgid, &msg, sizeof(msg), 1, 0);

    int number = atoi(msg.msg_text);
    if (isPalindrome(number)) {
        printf("Process B: The number %d is a palindrome.\n", number);
    } else {
        printf("Process B: The number %d is not a palindrome.\n", number);
    }

    // Destroy the message queue
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
