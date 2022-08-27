#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>


#define MAX_SNAKE_LEN 5
#define ROWS 10
#define COLS 10

struct termios saved_attributes;

void update_field(char field[ROWS][COLS]);

// Apply the saved terminal state
void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

// Displays the current state of the field
void update_field(char field[ROWS][COLS]) {

    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    /* if (field[i][j] != ' ')
		printf("* ");
		else*/
		printf("%c ", field[i][j]);
	}
	printf("\n");
    }
    
}

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

    // Center snake in the beginning
    field[ROWS/2][COLS/2] = snake_head;

    update_field(field);

    while (1) {

	c = getchar();

	switch (c) {

	case 'x':
	    return 1;
	}
	
    }
}

int main(void) {

    int answer;

    char *red = "\033[0;31m";
    char *green = "\033[0;32m";
    char *yellow = "\033[0;33m";
    char *reset = "\033[0m";

    while (1) {
	printf("---%sWelcome%s to %sTerminal%s %sSnake%s---\n\n", green, reset, yellow, reset, red, reset);

	printf("1. Play game\n"
	       "2. Exit\n\n"
	       "Pressing 'x' while in the game exists the game.\n\n");

	printf("Select option: ");
	scanf("%d", &answer);

	if (answer == 1) {
	    if(game() == 1)
		return 0;
	}
	else
	    break;

    }
    
    return 0;
}

