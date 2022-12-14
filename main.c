#include "string.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 3

bool playsX = false;
int playerScore = 0;
int cpuScore = 0;

const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int board[WIDTH][WIDTH];

// 0: Empty
// 1: X
// 2: O

struct vector { // Vector to store result
  int beginX;
  int beginY;
  int deltaX;
  int deltaY;
  bool playerLost;
  bool isValid;
};

void drawGrid(int grid[WIDTH][WIDTH]) {
  int i, j;

  for (i = 0; i < 3; i++) {
    // ABC 123 stuff
    if (i == 0) {
      printf("\n  1 2 3\n");
    }
    switch (i) {
    case 0:
      printf("A|");
      break;
    case 1:
      printf("B|");
      break;
    case 2:
      printf("C|");
      break;
    }

    // Board
    for (j = 0; j < 3; j++) {
      if (grid[i][j] == 0) {
        printf(" |");
      } else if (grid[i][j] == 1) {
        printf("X|");
      } else if (grid[i][j] == 2) {
        printf("O|");
      }
    }

    // New line
    printf("\n");
  }
}

void drawVictoryGrid(int grid[WIDTH][WIDTH], struct vector v) {
  int i, j;

  for (i = 0; i < 3; i++) {
    if (i == 0) {
      if (v.deltaX == v.deltaY) { // diagonal win
        if (v.beginX == 0) {
          printf("↘ 1 2 3\n");
        } else {
          printf("  1 2 3 ↙\n");
        }
      } else if (v.deltaX == 0 && v.deltaY == 2) {
        switch (v.beginX) {
        case 0:
          printf("  ↓ 2 3\n");
          break;
        case 1:
          printf("  1 ↓ 3\n");
          break;
        case 2:
          printf("  1 2 ↓\n");
          break;
        }
      } else {
        printf("  1 2 3\n");
      }
    }
    switch (i) {
    case 0:
      if (v.deltaX == 2 && v.deltaY == 0 && v.beginY == 0) {
        printf("→|");
      } else {
        printf("A|");
      }
      break;
    case 1:
      if (v.deltaX == 2 && v.deltaY == 0 && v.beginY == 1) {
        printf("\n→|");
      } else {
        printf("\nB|");
      }
      break;
    case 2:
      if (v.deltaX == 2 && v.deltaY == 0 && v.beginY == 2) {
        printf("\n→|");
      } else {
        printf("\nC|");
      }
      break;
    }

    for (j = 0; j < 3; j++) {
      if (grid[i][j] == 0) {
        printf(" |");
      } else if (grid[i][j] == 1) {
        printf("X|");
      } else if (grid[i][j] == 2) {
        printf("O|");
      }
    }
  }
  printf("\n");
}

void consume_rest_of_line(void) {
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) {
    ;
  }
}

void printScore() {
  printf("Score:\nPlayer - %d\nCPU - %d\n", playerScore, cpuScore);
}

void answerLoop(int *x, int *y) {
  while (1) {
    printf("\nEnter coordinates for your next move (ex.: B2), type 'score' for "
           "the current count: ");
    char answer[6];
    char score[6] = "score";
    fgets(answer, sizeof(answer), stdin);
    // answer[strlen(answer) - 1] = '\0';

    int jaap = strcmp(score, answer);
    if (jaap == 0) {
      consume_rest_of_line();
      printScore();
      continue;
    }

    // Determine validity of answer
    // First character
    if (answer[0] >= 'a' && answer[0] <= 'c') {
      answer[0] = answer[0] - 32; // Convert to upper case
    } else if (answer[0] >= 'A' && answer[0] <= 'C') {

    } else {
      printf("First character isn't a letter.");
      continue;
    }
    // Second character
    if (!(answer[1] >= '1' && answer[1] <= '3')) {
      printf("Second character isn't a number.");
      continue;
    }

    switch (answer[0]) {
    case 'A':
      *x = 0;
      break;
    case 'B':
      *x = 1;
      break;
    case 'C':
      *x = 2;
      break;
    }

    switch (answer[1]) {
    case '1':
      *y = 0;
      break;
    case '2':
      *y = 1;
      break;
    case '3':
      *y = 2;
      break;
    }

    break;
  }
}

// Returns a random number within a given range.
int randRange(int lower, int upper) {
  int num = (rand() % (upper - lower + 1)) + lower;
  return num;
}

struct vector gameIsOver() {
  // magic square trick to determine win
  // |4|9|2|
  // |3|5|7|
  // |8|1|6|

  int magicSquare[WIDTH][WIDTH] = {{4, 9, 2}, {3, 5, 7}, {8, 1, 6}};

  int i, j, sum;
  struct vector result;

  // Check cols
  for (i = 0; i < WIDTH; i++) {
    sum = 0;
    for (j = 0; j < WIDTH; j++) {
      sum += board[j][i] * magicSquare[j][i];
    }
    if (sum == 15) {
      printf("X won in collumn %d\n", i + 1);
      result.beginX = i;
      result.beginY = 0;
      result.deltaX = 0;
      result.deltaY = 2;
      result.playerLost = !playsX;
      result.isValid = true;
      return result;
    } else if (sum == 30) {
      printf("O won in collumn %d\n", i + 1);
      result.beginX = i;
      result.beginY = 0;
      result.deltaX = 0;
      result.deltaY = 2;
      result.playerLost = playsX;
      result.isValid = true;
      return result;
    }
  }

  // Check rows
  for (i = 0; i < WIDTH; i++) {
    sum = 0;
    for (j = 0; j < WIDTH; j++) {
      sum += board[i][j] * magicSquare[j][i];
    }
    if (sum == 15) {
      printf("X won in row %d\n", i + 1);
      result.beginX = 0;
      result.beginY = i;
      result.deltaX = 2;
      result.deltaY = 0;
      result.playerLost = !playsX;
      result.isValid = true;
      return result;
    } else if (sum == 30) {
      printf("O won in row %d\n", i + 1);
      result.beginX = 0;
      result.beginY = i;
      result.deltaX = 2;
      result.deltaY = 0;
      result.playerLost = playsX;
      result.isValid = true;
      return result;
    }
  }

  // Check diagonal
  sum = 0;
  for (i = 0; i < WIDTH; i++) {
    sum += board[i][i] * magicSquare[i][i];
  }
  if (sum == 15) {
    printf("X won in diag\n");
    result.beginX = 0;
    result.beginY = 0;
    result.deltaX = 2;
    result.deltaY = 2;
    result.playerLost = !playsX;
    result.isValid = true;
    return result;
  } else if (sum == 30) {
    printf("O won in diag\n");
    result.beginX = 0;
    result.beginY = 0;
    result.deltaX = 2;
    result.deltaY = 2;
    result.playerLost = playsX;
    result.isValid = true;
    return result;
  }

  // Check anti-diagonal
  sum = 0;
  for (i = 0; i < WIDTH; i++) {
    sum += board[i][(WIDTH - 1) - i] * magicSquare[i][(WIDTH - 1) - i];
  }
  if (sum == 15) {
    printf("X won in anti-diag\n");
    result.beginX = 2;
    result.beginY = 0;
    result.deltaX = 2;
    result.deltaY = 2;
    result.playerLost = !playsX;
    result.isValid = true;
    return result;
  } else if (sum == 30) {
    printf("O won in anti-diag\n");
    result.beginX = 2;
    result.beginY = 0;
    result.deltaX = 2;
    result.deltaY = 2;
    result.playerLost = playsX;
    result.isValid = true;
    return result;
  }

  result.isValid = false;
  return result;
}

void opponentTurn() {
  // Pick the center square by default if its empty.
  if (board[1][1] == 0) {
    printf("\nOpponent picked (0, 0)\n");
    board[1][1] = playsX ? 2 : 1;
    return;
  }

  // Look for a random open spot.
  while (1) {
    int moveX = randRange(0, 2);
    int moveY = randRange(0, 2);
    if (board[moveX][moveY] == 0) {
      printf("Opponent picked (%d, %d)\n", moveX, moveY);
      board[moveX][moveY] = playsX ? 2 : 1;
      break;
    }
  }
}

// Players turn
void playerTurn() {
  while (1) {
    int x, y;
    answerLoop(&x, &y);
    if (board[x][y] == 0) {
      printf("You picked (%d, %d)\n", x, y);
      board[x][y] = playsX ? 1 : 2;
      break;
    }
    printf("That spot isn't free!\n");
  }
}

void mainLoop() {
  int moveCount = 0;
  bool turnToggle = false;

  while (1) {
    // Draw the board
    drawGrid(board);

    if (turnToggle) {
      // CPUs turn
      opponentTurn();
    } else {
      playerTurn();
    }

    turnToggle = !turnToggle;
    moveCount++;

    // Check game state
    struct vector r = gameIsOver();
    if (r.isValid) {
      if (r.playerLost) {
        cpuScore++;
      } else {
        playerScore++;
      }
      drawVictoryGrid(board, r);
      break;
    }

    if (moveCount > 9) {
      printf("It's a draw...");
      break;
    }
  }
}

void clearScreen() {
  printf("\e[1;1H\e[2J"); // Clears the screen
}

int characterIsValid(char c) { // Determines if given character by user is a
                               // valid character, non-zero value means valid.
  if (c == 'X' || c == 'x') {
    clearScreen();
    printf("You picked the Xs.\n");
    playsX = true;
    return 1;
  } else if (c == 'O' || c == 'o') {
    clearScreen();
    printf("You picked the Os.\n");
    return 2;
  } else {
    printf("Invalid input. Please pick either Xs or Os by typing the letter");
    return 0;
  }
}

void characterSelect() {
  while (1) {
    char character;
    printf("\nPlay with Xs or Os?: ");
    character = fgetc(stdin);
    consume_rest_of_line();

    if (characterIsValid(character) > 0) {
      break;
    }
  }
}

bool continuePlaying() {
  while (1) {
    char c;
    printf("Would you like to continue? (y/n): ");
    c = fgetc(stdin);
    consume_rest_of_line();

    if (c == 'n') {
      return false;
    } else if (c == 'y') {
      return true;
    }

    printf("Please pick either 'y' or 'n'.\n");
  }
}

int main() {
  printf("Welcome to butter cheese and eggs in C.\n");
  srand(time(NULL)); // Give random number generator a seed.
  characterSelect();

  while (1) {
    memset(board, 0, sizeof(board[0][0]) * WIDTH * WIDTH); // Clear the board
    mainLoop();
    if (!continuePlaying()) {
      break;
    }
  }

  printScore();
}
