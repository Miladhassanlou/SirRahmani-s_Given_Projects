// Pacman Game in C language
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// All the elements to be used
// Declared here
#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'
#define ADD "E:/Coddings Related Files/Temp/pacamn_save.bin"

// Global Variables are
// Declared here
int res = 0;
int score = 0;
int pacman_x, pacman_y;
char board[HEIGHT][WIDTH];
int food = 0;
int curr = 0;

int isSaved()
{
	FILE *pfile = fopen(ADD, "rb");
	if (pfile != NULL)
	{
		return 1;
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
				board[i][j] = WALL;
			}
			else
				board[i][j] = EMPTY;
		}
	}

	// Putting Walls inside the Game
	int count = 50;
	while (count != 0)
	{
		int i = (rand() % (HEIGHT));
		int j = (rand() % (WIDTH));

		if (board[i][j] != WALL && board[i][j] != PACMAN)
		{
			board[i][j] = WALL;
			count--;
		}
	}

	int val = 5;
	while (val--)
	{
		int row = (rand() % (HEIGHT));
		for (int j = 3; j < WIDTH - 3; j++)
		{
			if (board[row][j] != WALL && board[row][j] != PACMAN)
			{
				board[row][j] = WALL;
			}
		}
	}

	// Putting Demons in the Game
	count = 10;
	while (count != 0)
	{
		int i = (rand() % (HEIGHT));
		int j = (rand() % (WIDTH));

		if (board[i][j] != WALL && board[i][j] != PACMAN)
		{
			board[i][j] = DEMON;
			count--;
		}
	}

	// Cursor at Center
	pacman_x = WIDTH / 2;
	pacman_y = HEIGHT / 2;
	board[pacman_y][pacman_x] = PACMAN;

	// Points Placed
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i % 2 == 0 && j % 2 == 0 && board[i][j] != WALL && board[i][j] != DEMON && board[i][j] != PACMAN)
			{

				board[i][j] = FOOD;
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
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	printf("Score: %d\n", score);
}

void move(int move_x, int move_y)
{
	int x = pacman_x + move_x;
	int y = pacman_y + move_y;

	if (board[y][x] != WALL)
	{
		if (board[y][x] == FOOD)
		{
			score++;
			food--;
			curr++;
			if (food == 0)
			{
				res = 2;
				return;
			}
		}
		else if (board[y][x] == DEMON)
		{
			res = 1;
		}

		board[pacman_y][pacman_x] = EMPTY;
		pacman_x = x;
		pacman_y = y;
		board[pacman_y][pacman_x] = PACMAN;
	}
}

void start(int *tFood)
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
	fwrite(tFood, sizeof(curr), 1, fp);

	fclose(fp);
}

int main()
{
	srand(time(NULL));
	int totalFood;
	start(&totalFood);
	char ch;
	char ch2;
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
		if (res == 1)
		{
			// Clear screen
			system("cls");
			printf("Game Over! Dead by Demon\n Your Score: "
				   "%d\n",
				   score);
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
			printf("Do you want to save the game?(if Yes press: Y)\n");
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
