// tic_tac_toe.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shm_com4.h"

void display_board(char *board) {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c \n", board[i * 3], board[i * 3 + 1], board[i * 3 + 2]);
        if (i < 2) {
            printf("---|---|---\n");
        }
    }
    printf("\n");
}

int check_winner(char *board) {
    // Check rows, columns, and diagonals
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] == board[i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2] && board[i * 3] != ' ')
            return 1; // Row winner
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6] && board[i] != ' ')
            return 1; // Column winner
    }
    if (board[0] == board[4] && board[4] == board[8] && board[0] != ' ')
        return 1; // Diagonal winner
    if (board[2] == board[4] && board[4] == board[6] && board[2] != ' ')
        return 1; // Diagonal winner
    return 0; // No winner
}

int main() {
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    int shmid;

    // Create shared memory
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    shared_stuff = (struct shared_use_st *)shared_memory;
    memset(shared_stuff->board, ' ', SIZE); // Initialize board
    shared_stuff->turn = 0; // Player 1 starts
    shared_stuff->game_over = 0;

    // Fork the process to create a child
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    }

    // Parent process
    if (pid > 0) {
        while (!shared_stuff->game_over) {
            display_board(shared_stuff->board);
            printf("Player 1 (X), enter your move (1-9): ");
            int move;
            scanf("%d", &move);
            move--; // Convert to 0-indexed

            if (move < 0 || move >= 9 || shared_stuff->board[move] != ' ') {
                printf("Invalid move. Try again.\n");
                continue;
            }

            shared_stuff->board[move] = 'X';
            shared_stuff->turn = 1; // Switch turn to Player 2

            if (check_winner(shared_stuff->board)) {
                display_board(shared_stuff->board);
                printf("Player 1 (X) wins!\n");
                shared_stuff->game_over = 1;
            } else if (strchr(shared_stuff->board, ' ') == NULL) {
                display_board(shared_stuff->board);
                printf("It's a draw!\n");
                shared_stuff->game_over = 1;
            }
        }

        // Wait for the child process to finish
        wait(NULL);
    } 
    // Child process
    else {
        while (!shared_stuff->game_over) {
            display_board(shared_stuff->board);
            printf("Player 2 (O), enter your move (1-9): ");
            int move;
            scanf("%d", &move);
            move--; // Convert to 0-indexed

            if (move < 0 || move >= 9 || shared_stuff->board[move] != ' ') {
                printf("Invalid move. Try again.\n");
                continue;
            }

            shared_stuff->board[move] = 'O';
            shared_stuff->turn = 0; // Switch turn to Player 1

            if (check_winner(shared_stuff->board)) {
                display_board(shared_stuff->board);
                printf("Player 2 (O) wins!\n");
                shared_stuff->game_over = 1;
            } else if (strchr(shared_stuff->board, ' ') == NULL) {
                display_board(shared_stuff->board);
                printf("It's a draw!\n");
                shared_stuff->game_over = 1;
            }
        }
    }

    // Detach and remove shared memory
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}

