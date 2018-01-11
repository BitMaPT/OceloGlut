#ifndef HEADER_SERVERBOARD
#define HEADER_SERVERBOARD

#include"ocelostone.h"

#define OCELO_WIDTH 8
#define OCELO_HEIGHT 8

int Initboard(int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CheckPutablePoints(int board[OCELO_HEIGHT][OCELO_WIDTH], int *list, OceloStoneColor color);

#endif