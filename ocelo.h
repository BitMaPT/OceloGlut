#ifndef HEADER_OCELO
#define HEADER_OCELO

#define OCELO_WIDTH 8
#define OCELO_HEIGHT 8

void PutStone(int x, int y, OceloStone stone);
void CheckAllSquaresToPut(OceloStone stone);

extern int ocelo[OCELO_HEIGHT][OCELO_WIDTH];
extern int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

#endif