#ifndef HEADER_OCELO
#define HEADER_OCELO

#define OCELO_WIDTH 8
#define OCELO_HEIGHT 8

typedef enum OceloStone {
  OCELO_STONE_WHITE,
  OCELO_STONE_BLACK,
  OCELO_STONE_NONE
}OceloStone;

void PutStone(int x, int y, OceloStone stone);
void CheckAllSquaresToPut(OceloStone stone);
int CanPut(int x, int y, OceloStone stone);
int CanPutRight(int x, int y, OceloStone stone, OceloStone enemy);
int CanPutLeft(int x, int y, OceloStone stone, OceloStone enemy);
int CanPutUp(int x, int y, OceloStone stone, OceloStone enemy);
int CanPutBelow(int x, int y, OceloStone stone, OceloStone enemy);
int CanPutUpRight(int x, int y, OceloStone stone, OceloStone enemy);
int CanPutUpLeft(int x, int y, OceloStone stone, OceloStone enemy);
int CanPutBelowRight(int x, int y, OceloStone stone, OceloStone enemy);
int CanPutBelowLeft(int x, int y, OceloStone stone, OceloStone enemy);

extern int ocelo[OCELO_HEIGHT][OCELO_WIDTH];
extern int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

#endif