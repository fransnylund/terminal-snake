#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>


#define ROWS 10
#define COLS 10

struct termios saved_attributes;

void update_field(char field[ROWS][COLS]);
void reset_terminal(void);
void place_food(char field[ROWS][COLS]);
void move(char field[ROWS][COLS], int dir_y, int dir_x); // dir_y(direction_y) and dir_x(direction_y)
int game(void);
int snake_x[ROWS * COLS] = {0};
int snake_y[ROWS * COLS] = {0};
int snake_len = 1;


// Apply the saved terminal state
void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

// Displays the current state of the field
void update_field(char field[ROWS][COLS]) {

    printf("\033[%dA", ROWS);
    printf("\033[%dD", COLS);

    // Insert the snake into the 2D array field
    for (int i = 0; i <= snake_len; i++) {
        field[snake_y[snake_len]][snake_x[snake_len]] = '*';
    }
    
    // Remove the old tail
    field[snake_y[0]][snake_x[0]] = ' ';

    // Draw whatever is in the field 2D array
    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    printf("%c ", field[i][j]);
	}
	printf("\n");
    }
    
}

// Place the food at a random position
void place_food(char field[ROWS][COLS]) {

    field[rand() % ROWS - 1][rand() % COLS - 1] = 'o';
}

void move(char field[ROWS][COLS], int dir_y, int dir_x) {

    if (dir_x == 1) {
        for (int i = 0; i < snake_len; i++) {
	    snake_x[i] = snake_x[i+1];
	    snake_y[i] = snake_y[i+1];
	}
	
	// If food on the right of the snake head
	if (field[snake_y[snake_len]][snake_x[snake_len] + 1] == 'o') {
	    snake_x[snake_len + 1] = snake_x[snake_len] + 1;
	    // Remember to change the y of the snake
	    snake_y[snake_len + 1] = snake_y[snake_len];
	    snake_len++;
	    // Place a new food after eating one
	    place_food(field);
	}
	else {
	    snake_x[snake_len]++;
	}
    }

    if (dir_x == -1) {
	 for (int i = 0; i < snake_len; i++) {
	    snake_x[i] = snake_x[i+1];
	    snake_y[i] = snake_y[i+1];
	}
	// If food on the left of the snake head
	if (field[snake_y[snake_len]][snake_x[snake_len] - 1] == 'o') {
	    snake_x[snake_len + 1] = snake_x[snake_len] - 1;
	    // Remember to change the y of the snake
	    snake_y[snake_len + 1] = snake_y[snake_len];
	    snake_len++;
	    // Place a new food after eating one
	    place_food(field);

	}
	else {
	    snake_x[snake_len]--;
	}
    }

    if (dir_y == -1) {
        for (int i = 0; i < snake_len; i++) {
	    snake_x[i] = snake_x[i+1];
	    snake_y[i] = snake_y[i+1];
	}
	// If food above the snake head
	if (field[snake_y[snake_len] - 1][snake_x[snake_len]] == 'o') {
	    snake_y[snake_len + 1] = snake_y[snake_len] - 1;
	    // Remember to change the x of the snake
	    snake_x[snake_len + 1] = snake_x[snake_len];
	    snake_len++;
	    // Place a new food after eating one
	    place_food(field);

	}
	else {
	    snake_y[snake_len]--;
	}
    }

    if (dir_y == 1) {
	for (int i = 0; i < snake_len; i++) {
	    snake_x[i] = snake_x[i+1];
	    snake_y[i] = snake_y[i+1];
	}
	// If food below the snake head
	if (field[snake_y[snake_len] + 1][snake_x[snake_len]] == 'o') {
	    snake_y[snake_len + 1] = snake_y[snake_len] + 1;
	    // Remember to change the x of the snake
	    snake_x[snake_len + 1] = snake_x[snake_len];
	    snake_len++;
	    // Place a new food after eating one
	    place_food(field);

	}
	else {
	    snake_y[snake_len]++;
	}
    }
}

// Where the game happens
int game() {

    char field[ROWS][COLS];
    char snake_head = '0';
    char c;

    struct termios term_attr;

    // Save terminal attributes and make exit() trigger reset_terminal()
    tcgetattr(STDIN_FILENO, &saved_attributes);
    atexit(reset_terminal);

    if (!isatty(STDIN_FILENO)) {
	fprintf(stderr, "ERROR: Not a terminal.\n");
	exit(1);
    }

    // Set terminal into non-canonical mode and disable echo
    tcgetattr(STDIN_FILENO, &term_attr);
    term_attr.c_lflag &= ~(ICANON|ECHO);
    term_attr.c_cc[VMIN] = 1;
    term_attr.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_attr);

    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    field[i][j] = ' ';
	}
    }

    // Position snake in starting pos
    field[ROWS/2][COLS/2] = snake_head;
    snake_x[snake_len] = COLS/2;
    snake_y[snake_len] = ROWS/2;

    srand((unsigned) time(NULL));

    // Position food in starting pos
    place_food(field);

    update_field(field);

    while (1) {

	c = getchar();


	switch (c) {

	case 'x':
	    return 1;
	case 'd':
	    move(field, 0, 1);
	    update_field(field);
	    break;
	case 'a':
	    move(field, 0, -1);
	    update_field(field);
	    break;
	case 'w':
	    move(field, -1, 0);
	    update_field(field);
	    break;
	case 's':
	    move(field, 1, 0);
	    update_field(field);
	    break;
	}

	printf("head: X: %d - Y: %d", snake_x[snake_len], snake_y[snake_len]);
	
    }
}

int main(void) {

    int answer;

    char *red = "\033[0;31m";
    char *green = "\033[0;32m";
    char *yellow = "\033[0;33m";
    char *reset = "\033[0m";

    while (1) {
	printf("\n---%sWelcome%s to %sTerminal%s %sSnake%s---\n\n", green, reset, yellow, reset, red, reset);

	printf("1. Play game\n"
	       "2. Exit\n\n"
	       "Pressing 'x' while in the game exists the game.\n\n");

	printf("Select option: ");
	scanf("%d", &answer);

	if (answer == 1) {
	    if(game() == 1)
		    // Break here instead of return?
		return 0;
	}
	else
	    break;

    }
    
    return 0;
}

