#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 3

void displayBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < SIZE - 1) printf("---|---|---\n");
    }
}

int checkWinner(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') return 1;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') return 1;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') return 1;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') return 1;
    return 0;
}

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, sizeof(char[SIZE][SIZE]), 0666 | IPC_CREAT);
    char (*board)[SIZE] = (char(*)[SIZE])shmat(shmid, (void *)0, 0);

    // Initialize board
    memset(board, ' ', sizeof(char[SIZE][SIZE]));
    int moves = 0;
    char player = 'X';

    while (moves < 9) {
        displayBoard(board);
        int row, col;
        printf("Player %c, enter your move (row and column): ", player);
        scanf("%d %d", &row, &col);

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != ' ') {
            printf("Invalid move, try again.\n");
            continue;
        }

        board[row][col] = player;
        moves++;

        if (checkWinner(board)) {
            displayBoard(board);
            printf("Player %c wins!\n", player);
            break;
        }

        player = (player == 'X') ? 'O' : 'X';
    }

    if (moves == 9) {
        printf("It's a draw!\n");
    }

    shmdt(board);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
