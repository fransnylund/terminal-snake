#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>


#define MAX_LEN 5 // Max snake length
#define ROWS 10
#define COLS 10

void update_field(char field[ROWS][COLS]);

// Displays the current state of the field
void update_field(char field[ROWS][COLS]) {

    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    printf("%c ", field[i][j]);
	}
	printf("\n");
    }
    
}


int main(void) {

    char field[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    field[i][j] = ' ';
	}
    }

    update_field(field);
    
    

    return 0;
}
