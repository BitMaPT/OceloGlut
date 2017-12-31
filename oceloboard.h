#ifndef HEADER_OCELO
#define HEADER_OCELO

#include"gamecontroller.h"

#define OCELO_WIDTH 8
#define OCELO_HEIGHT 8

void PutStone(int x, int y, OceloStoneColor stone);
void CheckAllSquaresToPut(OceloStoneColor stone);
int CanPut(int x, int y, OceloStoneColor stone);
int CanPutRight(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutLeft(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutUp(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutBelow(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutUpRight(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutUpLeft(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutBelowRight(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutBelowLeft(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);

extern Stone *oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
extern int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

#endif