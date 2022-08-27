#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>


#define MAX_SNAKE_LEN 5
#define ROWS 15
#define COLS 15

struct termios saved_attributes;

void update_field(void);
void reset_terminal(void);
void place_food(void);
void move(int dir_y, int dir_x); // dir_y(direction_y) and dir_x(direction_y)
int game(void);
int get_snake_len(void);
void get_head_pos(int *, int *);

char field[ROWS][COLS];
int snake_len = 0;
bool food = false;



// Apply the saved terminal state
void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

// Prints the current state of the field to the screen
void update_field(void) {

    printf("\033[%dA", ROWS);
    printf("\033[%dD", COLS);
    

    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    if (field[i][j] == 'o')
		food = true;
	    printf("%c ", field[i][j]);
	}
	printf("\n");
    }
    
}

// Place the food at a random position
void place_food(void) {

    field[rand() % ROWS][rand() & COLS] = 'o';
}

// Move the snake
// dir_y = direction up/down, dir_x = direction right, left
void move(int dir_y, int dir_x) {

    // pos_x = row
    // pos_y = col
    int pos_x, pos_y;
    // Place the current pos of head in pos_x and pos_y
    get_head_pos(&pos_x, &pos_y);
    
    bool right = false;
    bool left = false;
    bool up = false;
    bool down = false;

    if (dir_x == 1)
	right = true;
    else if (dir_x == -1)
	left = true;
    else if (dir_y == 1)
	down = true;
    else if (dir_y == -1)
	up = true;

    
    if (right) {
	if (field[pos_x][pos_y + dir_x] == 'o') {
	    food = false;
	    snake_len++;
	}
	for (int i = 0; i <= snake_len; i++) {
	    field[pos_x][pos_y + dir_x - i] = i + 48;
	    field[pos_x][pos_y - i] = ' ';
	    field[pos_x + dir_x - i][pos_y - i] = ' ';
	    field[pos_x + dir_x + i][pos_y - i] = ' ';	
	}
        
    }
    else if (left) {
	if (field[pos_x][pos_y + dir_x] == 'o') {
	    food = false;
	    snake_len++;
	}
	for (int i = 0; i <= snake_len; i++) {
	    field[pos_x][pos_y + dir_x + i] = i + 48;
	    field[pos_x][pos_y + i] = ' ';
	    field[pos_x + dir_x - i][pos_y + i] = ' ';
	    field[pos_x + dir_x + i][pos_y + i] = ' ';
	}
    }
    else if (up) {
	if (field[pos_x + dir_y][pos_y] == 'o'){
	    food = false;
	    snake_len++;
	}
	for (int i = 0; i <= snake_len; i++) {
	    field[pos_x + dir_y + i][pos_y] = i + 48;
	    field[pos_x + i][pos_y] = ' ';
	    field[pos_x + i][pos_y + dir_y - i] = ' ';
	    field[pos_x + i][pos_y + dir_y +i] = ' ';
	}
    }
    else if (down) {
	if (field[pos_x + dir_y][pos_y] == 'o') {
	    food = false;
	    snake_len++;
	}
	for (int i = 0; i <= snake_len; i++) {
	    field[pos_x + dir_y - i][pos_y] = i + 48;
	    field[pos_x - i][pos_y] = ' ';
	    field[pos_x - i][pos_y + dir_y - i] = ' ';
	    field[pos_x - i][pos_y + dir_y + i] = ' ';
	}
    }
    
}

// Get the position of the snake's head
void get_head_pos(int *i, int *j) {

    for (*i = 0; *i < ROWS; (*i)++) {
	for (*j = 0; *j < COLS; (*j)++) {
	    if (field[*i][*j] == '0')
		return;
	}
    }
}

// Where the game happens
int game(void) {

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

    // Initialize the field array to spaces
    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    field[i][j] = ' ';
	}
    }

    // Position snake in starting pos
    field[ROWS/2][COLS/2] = snake_head;

    srand((unsigned) time(NULL));

    // Position food in starting pos
    place_food();
    food = true;

    update_field();

    while (1) {

	c = getchar();

	switch (c) {

	case 'x':
	    return 1;
	case 'd':
	    move(0, 1);
	    update_field();
	    break;
	case 'a':
	    move(0, -1);
	    update_field();
	    break;
	case 'w':
	    move(-1, 0);
	    update_field();
	    break;
	case 's':
	    move(1, 0);
	    update_field();
	    break;
	}
	if (food == false) {
	    place_food();
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

	// If game() returns != 0, then it will not break out of the loop
	if (answer == 1) {
	    if(game() == 1)
		return 0;
	}
	else
	    break;

    }
    
    return 0;
}

