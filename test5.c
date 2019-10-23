#include <stdio.h>
#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include "conway.h"

int initall(void)
{
	initscr();			/* Start curses mode 		  */
	return 0;
};

int enditall(void)
{
	endwin();			/* End curses mode 		  */
	return 0;
};

// Create a 2D array with dimensions (size x size) using dynnammic memmory allocation
cell** createField(int fsizeX, int fsizeY)
{
  cell** field = (cell**) malloc(fsizeX*(sizeof(cell*)));
  for(int i=0; i<fsizeX; i++)
  {
    field[i] = (cell*) malloc(fsizeY*(sizeof(cell)));
  }
  return(field);
}

// Free the memmory in order to reduce memory  leaking
// -> free the memory for each row that has been created, and then the array
// pointer itself:
void freeField(cell** field, int fsizeX)
{
  for(int i=0; i<fsizeX; i++)
  {
    //field[i] = 0;
    free(field[i]);
  }
  //field = 0;
  free(field);
}

void initField(cell** field, int fsizeX, int fsizeY, int thr)
{
  uint min = 0;
  uint max = 100;
  uint val =  0;

  for(int i=1; i<fsizeX-1; i++)
  {
    for(int j=1; j<fsizeY-1; j++)
    {
      // Generate random number between 0 to 100
      int r = (rand() % (max-min)) + min;

      // Assign a 0 (dead) or a 1 (alive) according to a threshold
      if(r >= thr)
      {
        val = 1;
      } else {
        val = 0;
      }

      field[i][j].value = val;
      field[i][j].pos_x = i;
      field[i][j].pos_y = j;
    }
  }
}

int count8Neighbours(cell** field, int i, int j, int alive)
{
	int counter = 0;

	// THIS IS NICE BUT IT DOES NOT WORK!!
	// //For every cell in the field, count its neighbours (up to 8 neighbours)
	// for(int v = i-1; v < i+1; v++)
	// {
	//   for(int h = j-1; h < j+1; h++)
	//   {
	// 		counter += field[v][h].value;
	//   }
	// }
	//
	// // Substract the central element (current cell) beacuse we're only
	// //interested in its neighbours
	// counter -= field[i][j].value;

	//  Count the neighbours of the cell (up to 8 neighbours)
	if(field[i-1][j-1].value == 1){ counter++; }
	if(field[i][j-1].value == 1){ counter++; }
	if(field[i+1][j-1].value == 1){ counter++; }

	if(field[i-1][j].value == 1){ counter++; }
	if(field[i+1][j].value == 1){ counter++; }

	if(field[i-1][j+1].value == 1){ counter++; }
	if(field[i][j+1].value == 1){ counter++; }
	if(field[i+1][j+1].value == 1){ counter++; }

	return counter;
}

void applyConwayRules(cell** field, cell** nextField, int i, int j, int counter, int alive)
{
	/*Any live cell withfewer than two live neighbours dies, as if caused
	by under-population*/
	if( (alive == 1) && (counter < 2) )
	{
		nextField[i][j].value = 0;
	}

	/*Any live cell with two or three live neighbours lives on to the next
	generation*/
	else if( (alive == 1) && ( (counter == 2) || (counter == 3) ) )
	{
		nextField[i][j].value = 1;
	}

	/*Any live cell with more than three live neighbours dies, as if by
	over-population*/
	else if( (alive == 1) && (counter > 3) )
	{
		nextField[i][j].value = 0;
	}

	// Check for any dead cell
	else if( (alive == 0) && (counter == 3) )
	{
		nextField[i][j].value = 1;
	}

	else // for any other case
	{
		nextField[i][j].value = field[i][j].value;
	}
}

void updateField(cell** field, cell** nextField, int fsizeX, int fsizeY)
{
  for(int i=1; i<fsizeX-1; i++)
  {
    for(int j=1; j<fsizeY-1; j++)
    {
			int alive = field[i][j].value;
			int counter = count8Neighbours(field, i, j, alive);
			applyConwayRules(field, nextField, i, j, counter, alive);
    }
  }
}

void displayField(cell** field, int fsizeX, int fsizeY, int offsetX)
{
	// Visualize the field
	for (int i=1; i<fsizeX; i++)
	{
	  for (int j=1; j<fsizeY; j++)
	  {
			char c = ' ';
			if(field[i][j].value == 0)
			{
				c = ' ';
			}
			if(field[i][j].value == 1)
			{
				c = 254;
			}
	    // convert the value of the cell into a char
	    mvaddch(field[i][j].pos_x + offsetX, field[i][j].pos_y, c);
	    // ...and show it in screen
	    refresh();			/* Print it on to the real screen */
	  }
	}
}

void insertCreature(cell** field, int fsizeX, int fsizeY, int creature, int x, int y)
{
	switch(creature)
	{
		case 0: // R-pentomino type creature

			// Set the living cells of the creatures
			field[x][y-1].value = 1;
			field[x][y].value = 1;
			field[x+1][y].value = 1;
			field[x-1][y].value = 1;
			field[x-1][y+1].value = 1;

			break;

		case 1: // Diehard type creature

			// Set the living cells of the creatures
			field[x][y-3].value = 1;
			field[x][y-2].value = 1;
			field[x+1][y-2].value = 1;
			field[x-1][y+3].value = 1;
			field[x+1][y+3].value = 1;
			field[x+1][y+2].value = 1;
			field[x+1][y+4].value = 1;
			break;

		case 2: // Acorn type creature

			// Set the living cells of the creatures
			field[x-1][y-2].value = 1;
			field[x+1][y-3].value = 1;
			field[x+1][y-2].value = 1;
			field[x][y].value = 1;
			field[x+1][y+1].value = 1;
			field[x+1][y+2].value = 1;
			field[x+1][y+3].value = 1;
			break;
	}
}
