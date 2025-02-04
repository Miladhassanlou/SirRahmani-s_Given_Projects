#include <stdio.h>

#define SIZE 9

// Global Variables
int board[SIZE][SIZE];

int isValid(int row, int col, int number)
{
   for (int i = 0; i < SIZE; i++)
   {
      if (board[row][i] == number || board[i][col] == number)
         return 0;
   }

   int startRow = row - row % 3, startCol = col - col % 3;
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         if (board[i + startRow][j + startCol] == number)
            return 0;
      }
   }
   return 1;
}


int solvePuzzle()
{
   int r, c, found = 0;

   for (r = 0; r < SIZE; r++)
   {
      for (c = 0; c < SIZE; c++)
      {
         if (board[r][c] == 0)
         {
            found = 1;
            break;
         }
      }
      if (found)
         break;
   }

   if (!found)
      return 1; 

   for (int num = 1; num <= 9; num++)
   {
      if (isValid(r, c, num))
      {
         board[r][c] = num;
         if (solvePuzzle())
            return 1;
         board[r][c] = 0; 
      }
   }
   return 0; 
}


void displayBoard()
{
   for (int i = 0; i < SIZE; i++)
   {
      for (int j = 0; j < SIZE; j++)
      {
         printf("%d ", board[i][j]);
      }
      printf("\n");
   }
}

int main()
{
   
   int puzzle[SIZE][SIZE] = {
       {0, 0, 0, 0, 7, 0, 0, 3, 5},
       {0, 0, 0, 5, 9, 1, 0, 0, 6},
       {0, 6, 0, 0, 0, 0, 8, 9, 0},
       {3, 0, 0, 0, 6, 0, 0, 0, 8},
       {1, 0, 0, 3, 0, 8, 0, 0, 4},
       {6, 0, 0, 0, 2, 0, 0, 0, 7},
       {0, 8, 2, 0, 0, 0, 0, 6, 0},
       {5, 0, 0, 9, 1, 4, 0, 0, 0},
       {9, 7, 0, 0, 8, 0, 0, 0, 0}
   };

   if (solvePuzzle())
      displayBoard(); 
   else
      printf("can not be solved\n");

   return 0;
}
