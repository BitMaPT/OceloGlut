#ifndef HEADER_OCELO
#define HEADER_OCELO

#include "gamecontroller.h"
#include "ocelostone.h"

#define OCELO_WIDTH 8
#define OCELO_HEIGHT 8
#define OCELO_MARGIN 40
#define OCELO_SQUARE_SIZE 70

int PutStone(int x, int y, OceloStoneColor stone);
int CheckAllSquaresToPut(OceloStoneColor stone);
int GenerateSelectablePutPoint();
int CheckAllStoneReversed();
int MousePositionToSquarePosition(int x, int y, int *xx, int *yy);
int CheckPositionIsPutable(int x, int y);
void SetSelectablePutPoint(char *list);

extern Stone* oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
extern int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

#endif