#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#include "conway.h"

const int uDelay = 150000; // time delay in microseconds

int main(void)
{
  int offsetX = 2; // two lines of offset in X dirrection
  int thr = 100; // threshold in order to set more or less living cells in the field

  int fsizeX = 0; // field length
  int fsizeY = 0; // field width
  int tSim = 0; // simulation time
  int cType = 0; // creature type

  // User have to enter the parameters
  printf("Enter field's length (max. 50): ");
  scanf("%d", &fsizeY);
  printf("Enter field's length (max. 100): ");
  scanf("%d", &fsizeX);
  printf("Enter num. of generations: ");
  scanf("%d", &tSim);
  printf("Enter type of creature to simulate (from 0 to 2): ");
  scanf("%d", &cType);

  initall(); // init ncurses mode

  // Creation of the field of life
  cell** field = createField(fsizeX, fsizeY);

  initField(field, fsizeX, fsizeY, thr); // init a current field as zeros(thr = 100) or random (thr = [0, 99])

  printw("Creation of field of size %d x %d is completed", fsizeX, fsizeY);
  printw("\n");

  // Insert manually the creatures for its simulation
  insertCreature(field, fsizeX, fsizeY, cType, fsizeX/2, fsizeY/2);
  //insertCreature(field, fsizeX, fsizeY, cType, 20, 20);
  //insertCreature(field, fsizeX, fsizeY, cType, 20, 20);

  displayField(field, fsizeX, fsizeY, offsetX);

  //Init the simulation of game of life
  int t = 0;
  while(t < tSim)
  {
    printw("Generation num.: %d/%d", t, tSim-1);

    cell** nextField = createField(fsizeX, fsizeY);
    initField(nextField, fsizeX, fsizeY, 100); // init nextField as a field with all zeros

    updateField(field, nextField, fsizeX, fsizeY);
    displayField(nextField, fsizeX, fsizeY, offsetX);

    for(int i=0; i<fsizeX; i++)
    {
      for(int j=0; j<fsizeY; j++)
      {
        field[i][j].value = nextField[i][j].value;
      }
    }


    freeField(nextField, fsizeX); // Free the used allocated memory
    usleep(uDelay);
    t++;
  }

  freeField(field, fsizeX); // Free the used allocated memory
  usleep(uDelay);
  getch();			/* Wait for user input */
  enditall(); // end ncurses mode
}
