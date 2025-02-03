// Pacman Game in C language
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// All the elements to be used
// Declared here
#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'
#define BONUS '$'
#define ADD "E:/Coddings Related Files/Temp/pacamn_save.bin"

struct unit{
	char image;
	int isPass;
	int isDead;
	int foodValue;
};
// Global Variables are
// Declared here
int res = 0;
int score = 0;
int pacman_x, pacman_y;
struct unit board[HEIGHT][WIDTH];
int food = 0;
int curr = 0;
int countBonus=0;

int isSaved()
{
	FILE *pfile = fopen(ADD, "rb");
	if (pfile != NULL)
	{
		return 1;
		fclose(pfile);
	}
	else
	{
		return 0;
	}
}

void initialize()
{
	// Putting Walls as boundary in the Game
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0 || j == WIDTH - 1 || j == 0 || i == HEIGHT - 1)
			{
				board[i][j].image= WALL;
				board[i][j].isPass= 0;
				board[i][j].isDead= 0;
				board[i][j].foodValue= 0;
			}
			else
			{
				board[i][j].image = EMPTY;
				board[i][j].isPass= 1;
				board[i][j].isDead= 0;
				board[i][j].foodValue= 0;
			}
		}
	}

	// Putting Walls inside the Game
	int count = 50;
	while (count != 0)
	{
		int i = (rand() % (HEIGHT));
		int j = (rand() % (WIDTH));

		if (board[i][j].image != WALL && board[i][j].image != PACMAN)
		{
			board[i][j].image= WALL;
			board[i][j].isPass= 0;
			board[i][j].isDead= 0;
			board[i][j].foodValue= 0;
			count--;
		}
	}

	int val = 5;
	while (val--)
	{
		int row = (rand() % (HEIGHT));
		for (int j = 3; j < WIDTH - 3; j++)
		{
			if (board[row][j].image != WALL && board[row][j].image != PACMAN)
			{
				board[row][j].image= WALL;
				board[row][j].isPass= 0;
				board[row][j].isDead= 0;
				board[row][j].foodValue= 0;

			}
		}
	}

	// Putting Demons in the Game
	count = 10;
	while (count != 0)
	{
		int i = (rand() % (HEIGHT));
		int j = (rand() % (WIDTH));

		if (board[i][j].image != WALL && board[i][j].image != PACMAN)
		{
			board[i][j].image = DEMON;
			board[i][j].isPass = 1;
			board[i][j].isDead = 1;
			board[i][j].foodValue = 0;
			count--;
		}
	}

	// Cursor at Center
	pacman_x = WIDTH / 2;
	pacman_y = HEIGHT / 2;
	board[pacman_y][pacman_x].image = PACMAN;
	board[pacman_y][pacman_x].isPass= 1;
	board[pacman_y][pacman_x].isDead = 0;
	board[pacman_y][pacman_x].foodValue = 1;


	//Bonuses Placed
	count = 12;
	while (count != 0)
	{
		int i = (rand() % (HEIGHT));
		int j = (rand() % (WIDTH));

		if (board[i][j].image== EMPTY)
		{
			board[i][j].image = BONUS;
			board[i][j].isPass = 1;
			board[i][j].isDead = 0;
			board[i][j].foodValue = 0;
			count--;
		}
	}

	// Points Placed
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i % 2 == 0 && j % 2 == 0 && board[i][j].image != WALL && board[i][j].image != DEMON && board[i][j].image != PACMAN && board[i][j].image !=BONUS)
			{

				board[i][j].image = FOOD;
				board[i][j].isPass = 1;
				board[i][j].isDead = 0;
				board[i][j].foodValue = 1;
				food++;
			}
		}
	}
}

void draw()
{
	// Clear screen
	system("cls");

	// Drawing All the elements in the screen
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printf("%c", board[i][j].image);
		}
		printf("\n");
	}
	printf("Score: %d\n", score);
}

void move(int move_x, int move_y)
{ 	
	int i;
	if(countBonus>0)
	{
		i=2;
	}else
	{
		i=1;
	}

	for(;i>0;i--)
	{
		int x = pacman_x + move_x;
		int y = pacman_y + move_y;

		if (board[y][x].isPass ==1 )
		{
			if (board[y][x].foodValue > 0)
			{
				score += board[y][x].foodValue;
				food--;
				curr++;
				if (food == 0)
				{
					res = 2;
					return;
				}
			}
			else if (board[y][x].isDead == 1)
			{
				res = 1;
			}
			else if(board[y][x].image== BONUS)
			{
				countBonus=10+1;
			}

			board[pacman_y][pacman_x].image = EMPTY;
			board[pacman_y][pacman_x].isPass = 1;
			board[pacman_y][pacman_x].isDead = 0;
			board[pacman_y][pacman_x].foodValue = 0;

			pacman_x = x;
			pacman_y = y;
			
			board[pacman_y][pacman_x].image = PACMAN;
			board[pacman_y][pacman_x].isPass = 1;
			board[pacman_y][pacman_x].isDead = 0;
			board[pacman_y][pacman_x].foodValue =0 ;

		}
	}
	if(countBonus > 0)
	countBonus--;
}

void selfPlaying(int *tFood)
{
	if (isSaved())
	{
		printf("You have a saved game.\n");

		while (1)
		{
			printf("Choose one option:\n");
			printf("1-Resume saved game.   2-Start a New game.\n");
			char ch = getch();

			switch (ch)
			{
			case '1':
				FILE *fp = fopen(ADD, "rb");
				if (fp == NULL)
				{
					printf("Error reading file.");
					return;
				}
				fread(board, sizeof(board), 1, fp);
				fread(&pacman_x, sizeof(pacman_x), 1, fp);
				fread(&pacman_y, sizeof(pacman_y), 1, fp);
				fread(&score, sizeof(score), 1, fp);
				fread(&food, sizeof(food), 1, fp);
				fread(&curr, sizeof(curr), 1, fp);
				fread(tFood, sizeof(int), 1, fp);
				fread(&countBonus, sizeof(countBonus), 1, fp);
				fclose(fp);
				return;

			case '2':
				initialize();
				food -= 35;
				*(tFood) = food;
				return;

			default:
				printf("invalid choice.\n");
				break;
			}
		}
	}
	else
	{
		printf("starting New game...\n");
		initialize();
		food -= 35;
		*(tFood) = food;
	}
}

void saveGame(int *tFood)
{
	FILE *fp = fopen(ADD, "wb");
	if (fp == NULL)
	{
		printf("Error Saving Game in File!");
		return;
	}
	fwrite(board, sizeof(board), 1, fp);
	fwrite(&pacman_x, sizeof(pacman_x), 1, fp);
	fwrite(&pacman_y, sizeof(pacman_y), 1, fp);
	fwrite(&score, sizeof(score), 1, fp);
	fwrite(&food, sizeof(food), 1, fp);
	fwrite(&curr, sizeof(curr), 1, fp);
	fwrite(tFood, sizeof(int), 1, fp);
	fwrite(&countBonus, sizeof(countBonus), 1, fp);


	fclose(fp);
}

void autoPlaying()
{
	initialize(); 
	food -= 35; 
	int totalFood = food; 
	
	printf("Enter Y to Star autoPlaying:\n"); 

	char ch = getch(); 
	if (ch != 'Y' && ch != 'y') { 
		printf("Exit Game! "); 
		return ; 
	} 

	while (1) 
	{ 
		draw(); 
		printf("Total Food count: %d\n", totalFood); 
		printf("Total Food eaten: %d\n", curr); 
		if(countBonus>0)
		{
			printf("Bonus Movments: %d\n", countBonus);
		}
		if (res == 1) { 
			// Clear screen 
			system("cls"); 
			printf("Game Over! Dead by Demon\n Your Score:%d\n",score); 
			return ; 
		} 

		if (res == 2) { 
			// Clear screen 
			system("cls"); 
			printf("You Win! \n Your Score: %d\n", score); 
			return ; 
		} 

		// Find a random number for pacman's movment 
		int ch2 = rand()%4; 

		// Moving According to the 
		// Random Number 
		switch (ch2) { 
		case 0: 
			move(0, -1); 
			break; 
		case 1: 
			move(0, 1); 
			break; 
		case 2: 
			move(-1, 0); 
			break; 
		case 3: 
			move(1, 0); 
			break; 
		} 
		sleep(1);
		
		//exit automatic playing game
		if(kbhit())
		{
			char ch3=getch();
			if (ch3=='q'||ch3=='Q')
			{
			printf("Game Stopped! Game Score: %d\n", score); 
			return ; 
			}
			
		}
	} 

	return ;
}

int main()
{
	srand(time(NULL));
	int totalFood;

	printf("Choose one type of game:\n");
	printf("1-Automatic Playing by System. \t 2-Play Yourself. \t 3-Exit.\n");
	char ch2 = getch();
	switch(ch2)
	{
	case '1':
		autoPlaying();
		return 0;
	case '2':
		break;
	default:
		return 0;
	}
	
	selfPlaying(&totalFood);
	char ch;
	
	// Instructions to Play
	printf(" Use buttons for w(up), a(left) , d(right) and "
		   "s(down)\nAlso, Press Q for save and quit\n");

	printf("Enter Y to continue: \n");
	ch = getch();
	if (ch != 'Y' && ch != 'y')
	{
		printf("Exit Game! ");
		return 1;
	}

	while (1)
	{
		draw();
		printf("Total Food count: %d\n", totalFood);
		printf("Total Food eaten: %d\n", curr);
		if(countBonus>0)
		{
			printf("Bonus Movments: %d\n", countBonus);
		}
		
		if (res == 1)
		{
			// Clear screen
			system("cls");
			printf("Game Over! Dead by Demon\n Your Score:%d\n",score);
			return 1;
		}

		if (res == 2)
		{
			// Clear screen
			system("cls");
			printf("You Win! \n Your Score: %d\n", score);
			return 1;
		}

		// Taking the Input from the user
		ch = getch();

		// Moving According to the
		// input character
		switch (ch)
		{
		case 'w':
			move(0, -1);
			break;
		case 's':
			move(0, 1);
			break;
		case 'a':
			move(-1, 0);
			break;
		case 'd':
			move(1, 0);
			break;
		case 'q':
			system("cls");
			printf("Exiting game...\t Your Score: %d\n", score);
			printf("Do you want to save the game?(Press: Y)\n");
			ch2 = getch();
			if (ch2 == 'y' || ch2 == 'Y')
			{
				saveGame(&totalFood);
				printf("Game Saved and Exited!\n");
				return 0;
			}
			else
			{
				printf("Game Exited Successfully!");
				return 0;
			}
		}
	}

	return 0;
}
