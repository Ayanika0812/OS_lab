// player1.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "shm_com4.h"

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

    // Initialize game state
    memset(shared_stuff->board, ' ', SIZE);
    shared_stuff->turn = 0; // Player 1 starts
    shared_stuff->game_over = 0;

    while (!shared_stuff->game_over) {
        // Display the current board
        display_board(shared_stuff->board);

        // Wait for Player 1's turn
        while (shared_stuff->turn != 0) {
            sleep(1);
        }

        // Get Player 1's move
        int move;
        printf("Player 1 (X), enter your move (1-9): ");
        scanf("%d", &move);
        move--; // Convert to 0-indexed

        if (move < 0 || move >= 9 || shared_stuff->board[move] != ' ') {
            printf("Invalid move. Try again.\n");
            continue;
        }

        // Update the board and switch turn
        shared_stuff->board[move] = 'X';
        shared_stuff->turn = 1; // Switch turn to Player 2

        // Check for a win
        for (int i = 0; i < 3; i++) {
            if ((shared_stuff->board[i * 3] == 'X' && shared_stuff->board[i * 3 + 1] == 'X' && shared_stuff->board[i * 3 + 2] == 'X') ||
                (shared_stuff->board[i] == 'X' && shared_stuff->board[i + 3] == 'X' && shared_stuff->board[i + 6] == 'X')) {
                printf("Player 1 (X) wins!\n");
                shared_stuff->game_over = 1;
                break;
            }
        }
        if ((shared_stuff->board[0] == 'X' && shared_stuff->board[4] == 'X' && shared_stuff->board[8] == 'X') ||
            (shared_stuff->board[2] == 'X' && shared_stuff->board[4] == 'X' && shared_stuff->board[6] == 'X')) {
            printf("Player 1 (X) wins!\n");
            shared_stuff->game_over = 1;
        }
        // Check for draw
        if (strchr(shared_stuff->board, ' ') == NULL && !shared_stuff->game_over) {
            printf("It's a draw!\n");
            shared_stuff->game_over = 1;
        }
    }

    // Final display of the board
    display_board(shared_stuff->board);

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

