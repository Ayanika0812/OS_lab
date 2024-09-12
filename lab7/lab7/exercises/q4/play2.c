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

    // Get shared memory
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

    while (!shared_stuff->game_over) {
        // Display the current board
        display_board(shared_stuff->board);

        // Wait for Player 2's turn
        while (shared_stuff->turn != 1 && !shared_stuff->game_over) {
            sleep(1);
        }

        if (shared_stuff->game_over) break;

        // Get Player 2's move
        int move;
        printf("Player 2 (O), enter your move (1-9): ");
        scanf("%d", &move);
        move--; // Convert to 0-indexed

        if (move < 0 || move >= 9 || shared_stuff->board[move] != ' ') {
            printf("Invalid move. Try again.\n");
            continue;
        }

        // Update the board and switch turn
        shared_stuff->board[move] = 'O';
        shared_stuff->turn = 0; // Switch turn to Player 1

        // Check for a win
        if (check_winner(shared_stuff->board, 'O')) {
            printf("Player 2 (O) wins!\n");
            shared_stuff->game_over = 1;
        } else if (strchr(shared_stuff->board, ' ') == NULL) {
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
