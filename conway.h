
typedef struct
{
  int value;
  int pos_x;
  int pos_y;
} cell;

int initall(void);
int enditall(void);
cell** createField(int, int);
void freeField(cell**, int);
void initField(cell**, int, int, int);
void updateField(cell**, cell**, int, int);
void displayField(cell**, int, int, int);
void insertCreature(cell**, int, int, int, int, int);
int count8Neighbours(cell**, int, int, int);
void applyConwayRules(cell**, cell**, int, int, int, int);
