// shm_com.h
#define SIZE 9 // 3x3 grid

struct shared_use_st {
    char board[SIZE]; // Game board
    int turn;         // 0 for Player 1 (X), 1 for Player 2 (O)
    int game_over;    // 0 for ongoing, 1 for game over
};

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


int check_winner(char *board, char player) {
    // Check rows, columns, and diagonals for a winner
    for (int i = 0; i < 3; i++) {
        if ((board[i * 3] == player && board[i * 3 + 1] == player && board[i * 3 + 2] == player) ||
            (board[i] == player && board[i + 3] == player && board[i + 6] == player)) {
            return 1; // Winner found
        }
    }
    if ((board[0] == player && board[4] == player && board[8] == player) ||
        (board[2] == player && board[4] == player && board[6] == player)) {
        return 1; // Winner found
    }
    return 0; // No winner
}
