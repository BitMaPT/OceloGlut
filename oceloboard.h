#ifndef HEADER_OCELO
#define HEADER_OCELO

#include "gamecontroller.h"
#include "ocelostone.h"

#define OCELO_WIDTH 8
#define OCELO_HEIGHT 8

void PutStone(int x, int y, OceloStoneColor stone);
int CheckAllSquaresToPut(OceloStoneColor stone);
int GenerateSelectablePutPoint();

extern Stone* oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
extern int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

#endif