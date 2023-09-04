#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define COLS 7
#define LINES 6
#define OK 1
#define AGAIN 2
#define QUIT 3
#define WIN 4
#define DRAW 5

struct pattern
{
  unsigned columns;
  unsigned lines;
};

char grid[COLS][LINES];

#include "connect.h"

int main(void)
{
  time_t t;
  if(time(&t) == (time_t)-1)
  {
    fprintf(stderr,"Error while calling the time function !\n");
    return EXIT_FAILURE;
  }
  srand((unsigned)t);
  grillInit();
  int value,status,pieces = 'O';
  printf("How many players ?\n>");
  int players;
  while(1)
  {
    int status = input(&players,1,2);
    if(status != OK)
    {
      if(status == AGAIN)
      {
        if(!empty(stdin))
        {
          fprintf(stderr,"Error while emptying the stream !\n");
          return EXIT_FAILURE;
        }
        continue;
      }
      else
      {
        return 0;
      }
    }
    break;
  }
  grillOutput();
  while (1)
  {
    struct pattern current;
    if(players == 1 && pieces == 'X')
    {
      value = ai() + 1;
      printf("ai : %d\n",value);
    }
    else
    {
      printf("Player %d >",(pieces == 'O') ? 1 : 2);
      status = input(&value,1,7);
      if(status != OK)
      {
        if(status == AGAIN)
        {
          if(empty(stdin) != OK)
          {
            fprintf(stderr,"Error while empty the stream !\n");
            return EXIT_FAILURE;
          }
          continue;
        }
        else
        {
          return 0;
        }
      }
    }
    position(&current,value -  1);
    grillFiller(&current,pieces);
    grillOutput();
    status = state(&current,pieces);
    if(status !=OK)
    {
      break;
    }
    pieces = (pieces == 'O') ? 'X' : 'O';
  }
  if (status == WIN)
  {
    fprintf(stderr,"The player %d win the game !\n",(pieces == 'O') ? 1 : 2);
  }
  else
  {
    fprintf(stderr,"The grid is full and no one win the game !\n");
  }
  
  return 0;
}

int input(int *value,int a,int b)
{
  while(!scanf("%d",value))
  {
    char letter;
    while (scanf("%c",&letter))
    {
      switch(letter)
      {
        case 'q':
        case 'Q':
          return QUIT;
        default:
          return AGAIN;
      }
    }
  }
  if(*value < a || *value > b)
  {
    return AGAIN;
  }
  return OK;
}

void grillOutput(void)
{
  for(int i=1;i<=COLS;i++)
    printf("  %d ",i);
  putchar('\n');

    for (int j = 0; j < LINES; j++)
    {
      putchar('+');
      for (int k = 0; k < COLS; k++)
      {
        printf("---+");
      }
      putchar('\n');
      putchar('|');
      for(int l = 0;l < COLS;l++)
      {
        printf(" %c |",grid[l][j]);
      }
    putchar('\n');
  }
  putchar('+');
  for (int k = 0; k < COLS; k++)
  {
    printf("---+");
  }
  putchar('\n');
  for(int i=1;i<=COLS;i++)
    printf("  %d ",i);
  putchar('\n');
}

void grillInit(void)
{
  for (int i = 0; i < COLS; i++)
  {
    for (int j = 0; j < LINES; j++)
    {
      grid[i][j] = ' ';
    }
  }
}

void position(struct pattern *current,int column)
{
  int line = LINES - 1;
  while(grid[column][line] != ' ' && line > 0)
  {
    line--;
  }
  current->columns = column;
  current->lines = line;
}

void grillFiller(struct pattern *current,int pieces)
{
  grid[current->columns][current->lines] = pieces;
}

int stateCheck(struct pattern *current, int pieces) 
{
    //horizontal align check
    int max = dirStateCheck(1,0,pieces,current) + dirStateCheck(-1,0,pieces,current) - 1;
    //Vertical align check
    max = maximal(max,dirStateCheck(0,1,pieces,current) + dirStateCheck(0,-1,pieces,current) - 1);
    //diagonally
    max = maximal(max,dirStateCheck(1,1,pieces,current) + dirStateCheck(-1,-1,pieces,current) - 1);
    max = maximal(max,dirStateCheck(-1,1,pieces,current) + dirStateCheck(1,-1,pieces,current) - 1);
    return max;
}

int dirStateCheck(int horizontal, int vertical, int pieces, struct pattern *current)
{
    int i = 1;
    int vert = current->lines + vertical;
    int hor = current->columns + horizontal;
    if(!positionCheck(hor,vert))
      {
        return i;
      }
    while(grid[hor][vert] == pieces) {
      if(!positionCheck(hor,vert))
      {
        break;
      }
      i++;
      vert += vertical;
      hor += horizontal;
    }
    return i;
}

bool positionCheck(int cols,int lines)
{
    if (cols > 6 || cols < 0) {
        return false;
    }
    else if(lines > 5 || lines < 0)
    {
        return false;
    }
    return true;
}

int maximal(int a,int b)
{
  return (a > b) ? a : b;
}

bool gridFull(void)
{
  for (int j = 0; j < COLS; j++)
  {
    if (grid[j][0] == ' ')
    {
      return false;
      break;
    }
  }
    return true;
}

int ai(void)
{
  int max = 0;
  int length;
  int bestColumn;
  int bestColumns[COLS];
  for(int i = 0;i < COLS;i++)
  {
    struct pattern current;
    
    if(grid[i][0] != ' ')
      continue;

    position(&current,i);
    length = stateCheck(&current,'O');

    if(length >= 4)
      return i;

    length = maximal(length,stateCheck(&current,'X'));

    if(length >= max)
    {
      if(length > max)
      {
        bestColumn = 0;
        max = length;
      }
      bestColumns[bestColumn] = i;
      bestColumn++;
    }
  }
  return bestColumns[randNum(0,bestColumn)];
}

double random(void)
{
  return rand() / (RAND_MAX + 1.);
}

int randNum(int min,int max)
{
  return random() * (max - min + 1) + min;
}

int empty(FILE *stream)
{
  int c;
  do
  {
    c = fgetc(stream);
  } while (c != '\n' || c != EOF);
  return ferror(stream) ? QUIT : OK;
}

int state(struct pattern *current,int pieces)
{
  int val = stateCheck(current,pieces);
  if (val >= 4 )
  {
    return WIN;
  }
  else if (gridFull())
  {
    return DRAW;
  }
  
  return OK;
}