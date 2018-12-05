///////////////////////////////////////////////////////
// Meluleki Dube
// 29 November 2018
// Snake game
//////////////////////////////////////////////////////

//includes
#include<stdio.h>
#include<curses.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <stdbool.h>
//end of includes

//static variables for use in class
static const int BOUNDARY_HEIGHT = 25;
static const int BOUNDARY_WIDTH  = 60; 
static const int BOUNDARY_HEIGHT_MID = 12;
static const int BOUNDARY_WIDTH_MID = 30;
static char user_name[100];

static int score = 0;
static int length = 0;

typedef char* _string;	

//determines the direction of the blocks of the snake
enum Direction{STILL, RIGHT, LEFT, UP, DOWN};

//this stores the row and col location of the block
typedef struct coordinate
{
	int row;
	int col;
	enum Direction dir;
}Cordinates;


//this is describes the smallest elemt that makes the snake.
typedef struct Block
{
	Cordinates location;
	enum Direction dir;
	char indicator;
	struct Block* next;
} Block;

typedef struct Instruction
{
	Cordinates location;
	enum Direction dir;
	struct Instruction *next;
} command;

Block* HEAD;
Block* FOOD;
command* c_HEAD = NULL; 
command* c_TAIL = NULL;
/***** begin initialization methods */ 
void welcome_message()
{
	clear();
	addstr("	Game instructions:");
	move(2,0);
	addstr("->: use the arrows to move the snake");
	move(4, 0);
	addstr("->: You will be provided with food(*). ");
	addstr("Eating the food will mean that the snake will grow in size and hence more points");
	move(7, 0);
	addstr("->: to gain more points try as much food as possible.");
	move(9, 0);
	addstr("-> Avoid having the snake bite it selft or hitting boundaries.");
	move(13, 0);
	addstr("	Press any key to continue!!!!!!!!!!!!!");
	move(15, 0);
	refresh();
	getchar();
}

/*
void change_color()
{
	start_color();			 //Start color 
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	attroff(COLOR_PAIR(1));
}
*/

void get_name(void)
{
	(void)echo();
	addstr( "What is your name> " );
	refresh();
	getnstr( user_name, sizeof( user_name ) - 1 );
}

Block* new_block(Block* prev)
{
	Block* blc = (struct Block*)malloc(sizeof(Block));
	blc->indicator = 254;
	Cordinates cod = prev->location;
	switch(prev->dir)
	{
		case RIGHT:
			cod.col = prev->location.col-1;
			break;
		case LEFT:
			cod.col =prev->location.col+1;
			break;
		case DOWN:
			cod.row = prev->location.row+1;
			break;
		case UP:
			cod.row = prev->location.row-1;
			break;
	}
	blc->location = cod;
	blc->dir = prev->dir;
	blc->next = NULL;
	return blc;
}

void draw_snake()
{
	Block *curr = HEAD;
	while(curr != NULL)
	{
		mvprintw(curr->location.row, curr->location.col, "%c", curr->indicator);
		curr = (curr->next);
	}
	move(BOUNDARY_HEIGHT+1,0);
	refresh();
}

void draw_food(void)
{
	mvprintw(FOOD->location.row, FOOD->location.col, "%c", FOOD->indicator);
	move(BOUNDARY_HEIGHT+1,0);
	refresh();
}

void init_snake(void)
{
	HEAD = (Block*)malloc(sizeof(Block));
	HEAD->indicator = 254;
	Cordinates cod = {2+BOUNDARY_HEIGHT_MID, BOUNDARY_WIDTH_MID};
	HEAD->location = cod;
	HEAD->dir = RIGHT;
	HEAD->next = new_block(HEAD);
	draw_snake();
}

void init_food(void)
{
	FOOD = (Block*)malloc(sizeof(Block));
	FOOD->indicator = 254;
	int x, y;
	x = rand() % BOUNDARY_WIDTH;
	y = rand() % BOUNDARY_HEIGHT;
	Cordinates cod = {y, x};
	FOOD->location = cod;
	FOOD->dir = STILL;
	draw_food();
}

void init_term(void)
{
	get_name();
	const char boundary[] = "*************************************************************";
	clear();
	addstr("	Name: ");
	addstr(user_name);
	move(0, 30);
	printw("Score: %d", score);
	mvaddstr(2, 0, boundary);
	int i = 3;
	for(; i < BOUNDARY_HEIGHT; ++i)
	{
		mvaddch(i, 0, boundary[0]);
		mvaddch(i, BOUNDARY_WIDTH, boundary[0]);
	}
	mvaddstr(i, 0, boundary);
	refresh();
	init_snake();
	init_food();
}

//free the memory we used
void free_mem()
{
	free(FOOD);
	Block* curr = HEAD;
	Block* temp;
	while(curr != NULL)
	{
		temp = curr;
		curr = curr->next;
		free(temp);
	}
}
/***** end of initialization methods */ 

bool cont_play = TRUE;

/**  declaration of methods to playing of game **/
void pause(void);
void play(void);
void delay(void);
void check_command(void);
void move_snake(void);

int main(void)
{
	setlocale(LC_ALL,"");
	initscr(); //creates stdscreen
	welcome_message();
	init_term();
	play();
	free_mem();
	endwin(); // this freees memory. think of it as destructor
	return 0;
}

void delay()
{
	for(int i=0; i < 100000000; ++i);
}

void check_command(void) 
{ 
    // Store the value argument passed to this thread 
    //int *myid = (int *)vargp; 
	int c = -10;
	c= getch();
	enum Direction dir;
	switch(c)
	{
		 case KEY_DOWN:
			dir = DOWN;
			break;
		 case KEY_UP:
			dir = UP;
			break;
		 case KEY_LEFT:
			dir = LEFT;
			break;
		 case KEY_RIGHT:
			dir = RIGHT;
			break;
		 case 27:
			cont_play = FALSE;
			break;
		 default:
			c = -10;
	}
	if(c != -10 && c != 27)
	{
		if(c_HEAD == NULL)
		{
			c_HEAD = (command*)malloc(sizeof(command));
			c_HEAD->location = HEAD->location;
			c_HEAD->dir = dir;
			c_HEAD->next = NULL;
			c_TAIL = c_HEAD;
		}
		else
		{			
			command* cmd = (command*)malloc(sizeof(command));
			cmd->location = HEAD->location;
			cmd->dir = dir;
			cmd->next = NULL;
			c_TAIL->next = cmd;
			c_TAIL = cmd;
		}
	}
	
}

void play(void)
{
	nodelay(stdscr, TRUE);
	keypad(stdscr,TRUE);
	int i = 0;
	while(1)
	{
		move_snake();
		check_command();
		if(!cont_play)
		{
			return;
		}
		delay();
	}
}

void check_dir(Block* b)
{
	command *temp = c_HEAD;
	while(temp != NULL)
	{
		if(temp->location.row == b->location.row && temp->location.col == b->location.col)
		{
			b->dir = temp->dir;
			return;
		}
		temp = temp->next;
	}
}

void move_snake()
{
	Block* curr = HEAD;
	while(curr != NULL)
	{
		
		mvaddch(curr->location.row, curr->location.col, ' ');
		check_dir(curr);
		switch(curr->dir)
		{
			case RIGHT:
				++curr->location.col;
				break;
			case LEFT:
				--curr->location.col;
				break;
			case DOWN:
				++curr->location.row;
				break;
			case UP:
				--curr->location.row;
		}
		curr = curr->next;
	
	}
	draw_snake();
}