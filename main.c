#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    char color; // 'A' = white, 'N' = black
    char size; // number of pieces on a point
} Point;

#define NUM_POINTS 24

void initializeBoard(Point board[]) {
    int i = 0;
    for (i = 1; i <= NUM_POINTS; i++) {
        board[i].size = 0;
        board[i].color = ' ';
    }
    board[13].size = 5;
    board[13].color = 'A';
    board[12].size = 5;
    board[12].color = 'N';
    board[17].size = 3;
    board[17].color = 'N';
    board[8].size = 3;
    board[8].color = 'A';
    board[19].size = 5;
    board[19].color = 'N';
    board[6].size = 5;
    board[6].color = 'A';
    board[24].size = 2;
    board[24].color = 'A';
    board[1].size = 2;
    board[1].color = 'N';

}

void printBoard(Point tabla[], int maxTop, int maxBottom, int hitW, int hitB) {
    printf("13 14 15 16 17 18  |%d|  19 20 21 22 23 24\n\n", hitB);
    int i, j, k = 0;
    for (i = 0; i < maxTop; i++) {
        for (j = 13; j <= 18; j++) {
            if (tabla[j].size - i > 0) {
                printf(" %c ", tabla[j].color);
            } else {
                printf("   ");
            }
        }
        printf("      ");
        for (j = 19; j <= 24; j++) {
            if (tabla[j].size - i > 0) {
                printf(" %c ", tabla[j].color);
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("\n");
    for (i = maxBottom - 1; i >= 0; i--) {
        for (j = 12; j >= 7; j--) {
            if (tabla[j].size - i > 0) {
                printf(" %c ", tabla[j].color);
            } else {
                printf("   ");
            }
        }
        printf("      ");
        for (j = 6; j >= 1; j--) {
            if (tabla[j].size - i > 0) {
                printf(" %c ", tabla[j].color);
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("\n12 11 10  9  8  7  |%d|   6  5  4  3  2  1\n", hitW);
}

void makeMove (Point board[], int src, int dest, char player, int dices[2], int *hitW, int *hitB) {
    board[src].size--;
    if (dest == 0) {
        return;
    }
    if(board[src].size == 0)
        board[src].color = ' ';
    if(board[dest].size == 0) {
        board[dest].size++;
        board[dest].color = player;
    }
    else if(board[dest].size == 1) {
        if(player == 'N') {
            if(board[dest].color == 'A') {
                *hitW++;
                board[dest].color = player;
                board[dest].size = 1;
            }
            else {
                board[dest].size++;
            }
        }
        else if(player == 'A') {
            if(board[dest].color == 'B') {
                *hitB++;
                board[dest].color = player;
                board[dest].size = 1;
            }
            else {
                board[dest].size++;
            }
        }
    }
}

int outstideHome(Point board[], char player,  int *hitW, int *hitB) {
    if (player == 'A') {
        if (*hitW) {
            return 1;
        }
        for (int i = 7; i <= 24; i++) {
            if (board[i].size && board[i].color == 'A') {
                return 1;
            }
        }
    }
    if (player == 'N') {
        if (*hitB) {
            return 1;
        }
        for (int i = 1; i <= 18; i++) {
            if (board[i].size && board[i].color == 'N') {
                return 1;
            }
        }
    }
    return 0;
}

int checkLegalMove (Point board[], int src, int dest, char player, int dices[2], int *hitW, int *hitB) {
    if (dest == 0) {
        if (outstideHome(board, player, hitW, hitB)) {
            return 0;
        }
    }
    if (src < 1 || src > 24 || dest < 1 || dest > 24) {
        return 0;
    }
    int dist = abs(src - dest);
    if (dist != dices[0] && dist != dices[1]) {
        return 0;
    }
    if (player == 'A') { // white
        if (board[src].size == 0) {
            return 0;
        }
        if (*hitW) {
            if (dest < 19) {
                return 0;
            }
        }
        if (board[dest].size > 1 && board[dest].color == 'N') {
            return 0;
        }
        if (dest >= src) {
            return 0;
        }
    }
    if (player == 'N') { // black
        if (board[src].size == 0) {
            return 0;
        }
        if (*hitB) {
            if (dest > 6) {
                return 0;
            }
        }
        if (board[dest].size > 1 && board[dest].color == 'A') {
            return 0;
        }
        if (dest <= src) {
            return 0;
        }
    }
    return 1;
}

void diceRoll (int *moves, int dices[2]) {
    srand(time(NULL));
    dices[0] = (rand() % 6) + 1;
    dices[1] = (rand() % 6) + 1;
    if(dices[0] == dices[1])
        *moves = 4;
    else
        *moves = 2;
}

int checkGameEnd (Point board[]) {
    int blackCount = 0, whiteCount = 0;
    for (int i = 0; i < NUM_POINTS; i++) {
        if (board[i].color == 'N') {
            blackCount += board[i].size;
        }
        if (board[i].color == 'A') {
            whiteCount += board[i].size;
        }
    }
    if (blackCount == 0) {
        return 2;
    } else if (whiteCount == 0) {
        return 1;
    }
    return 0;
}

int getMaxTop (Point board[]) {
    int max = 0, i = 0;
    for (i = 13; i <= 24; i++) {
        if (board[i].size > max)
            max = board[i].size;
    }
    return max;
}

int getMaxBottom (Point board[]) {
    int max = 0, i = 0;
    for (i = 1; i <= 12; i++) {
        if (board[i].size > max)
            max = board[i].size;
    }
    return max;
}

int main() {
    Point board[NUM_POINTS + 1];
    initializeBoard(board);
    printBoard(board, 5, 5, 0, 0);
    char player; // 'A' == white, 'N' == black
    int dices[2] = {0,0};
    int moves;
    int hitW = 0, hitB = 0;
    char key;
    int playerWon;
    int maxTop, maxBottom;
    int src, dest;
    while (dices[0] == dices[1]) {
        printf ("Dice roll (press Enter) :\n");
        scanf ("%c", &key);
        diceRoll(&moves, dices);
        printf("White rolled: %d\n", dices[0]);
    
        printf("Black rolled: %d\n", dices[1]);
        if (dices[0] == dices[1]) {
            printf("Roll again!");
        }
    }
    if (dices[0] > dices[1]) {
        player = 'A';
    } else {
        player = 'N';
    }
    playerWon = 0;
    while (!playerWon) {
        if (player == 'A') {
            printf("White's turn. Roll the dice (press Enter):\n");
        } else {
            printf("Black's turn. Roll the dice (press Enter):\n");
        }
        scanf("%c", &key);
        diceRoll(&moves, dices);        
        printf("You rolled: %d %d\n", dices[0], dices[1]);
        while (moves) {
            maxTop = getMaxTop(board);
            maxBottom = getMaxBottom(board);
            printBoard(board, maxTop, maxBottom, hitW, hitB);
            printf("Moves remaining : %d\n", moves);
            printf("Input move (source destination) :\n");
            scanf("%d%d", &src, &dest);
            if (checkLegalMove(board, src, dest, player, dices, &hitW, &hitB)) {
                makeMove(board, src, dest, player, dices, &hitW, &hitB);
            } else {
                printf("Illegal move. Try again\n");
                moves++;
            }
            moves--;
        }
        playerWon = checkGameEnd(board);
        if (player == 'A')
            player = 'N';
        else 
            player = 'A';
    }
    if (playerWon == 1) {
        printf("White won! Congratulations!\n");
    } else {
        printf("Black won! Congratulations!\n");
    }
    return 0;
}
