#define OCELO_WIDTH 8
#define OCELO_HEIGHT 8

typedef enum OceloStone {
  STONE_WHITE,
  STONE_BLACK
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


int ocelo[OCELO_HEIGHT][OCELO_WIDTH];
int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

void PutStone(int x, int y, OceloStone stone) {
  int i;
  OceloStone enemy;

  if(stone == STONE_WHITE) enemy = STONE_BLACK;
  if(stone == STONE_BLACK) enemy = STONE_WHITE;

  ocelo[y][x] = stone;

  if(CanPutRight(x, y, stone, enemy)) {
    for(i = x + 1; i < OCELO_WIDTH; i++) {
      if(ocelo[y][i] == stone) break;
      ocelo[y][i] = stone;
    }
  }

  if(CanPutLeft(x, y, stone, enemy)) {
    for(i = x - 1; i >= 0; i--) {
      if(ocelo[y][i] == stone) break;
      ocelo[y][i] = stone;
    }
  }

  if(CanPutUp(x, y, stone, enemy)) {
    for(i = y - 1; i >= 0; i--) {
      if(ocelo[i][x] == stone) break;
      ocelo[i][x] = stone;
    }
  }

  if(CanPutBelow(x, y, stone, enemy)) {
    for(i = y + 1; i < OCELO_HEIGHT; i++) {
      if(ocelo[i][x] == stone) break;
      ocelo[i][x] = stone;
    }
  }

  if(CanPutUpRight(x, y, stone, enemy)) {
    for(i = 1; (x + i < OCELO_WIDTH) && (y - i >= 0); i++) {
      if(ocelo[y - i][x + i] == stone) break;
      ocelo[y - i][x + i] = stone;
    }
  }

  if(CanPutUpLeft(x, y, stone, enemy)) {
    for(i = 1; (x - i >= 0) && (y - i >= 0); i++) {
      if(ocelo[y - i][x - i] == stone) break;
      ocelo[y - i][x - i] = stone;
    }
  }

  if(CanPutBelowRight(x, y, stone, enemy)) {
    for(i = 1; (x + i < OCELO_WIDTH) && (y + i < OCELO_HEIGHT); i++) {
      if(ocelo[y + i][x + i] == stone) break;
      ocelo[y + i][x + i] = stone;
    }
  }

  if(CanPutBelowLeft(x, y, stone, enemy)) {
    for(i = 1; (x - i >= 0) && (y + i < OCELO_HEIGHT); i++) {
      if(ocelo[y + i][x - i] == stone) break;
      ocelo[y + i][x - i] = stone; 
    }
  }
}

void CheckAllSquaresToPut(OceloStone stone) {
  int x, y;

  for(x = 0; x < OCELO_WIDTH; x++) {
    for(y = 0; y < OCELO_HEIGHT; y++) {
      oceloCanPut[y][x] = CanPut(x, y, stone);
    }
  }
}

int CanPut(int x, int y, OceloStone stone) {
  int i;
  OceloStone enemy;

  if(stone == STONE_WHITE) enemy = STONE_BLACK;
  if(stone == STONE_BLACK) enemy = STONE_WHITE;

  if(CanPutRight(x, y, stone, enemy)) return 1;
  if(CanPutLeft(x, y, stone, enemy)) return 1;
  if(CanPutUp(x, y, stone, enemy)) return 1;
  if(CanPutBelow(x, y, stone, enemy)) return 1;
  if(CanPutUpRight(x, y, stone, enemy)) return 1;
  if(CanPutUpLeft(x, y, stone, enemy)) return 1;
  if(CanPutBelowRight(x, y, stone, enemy)) return 1;
  if(CanPutBelowLeft(x, y, stone, enemy)) return 1;

  return 0;
}

int CanPutRight(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(ocelo[y][x + 1] != enemy) return 0;

  for(i = x + 1; i < OCELO_WIDTH; i++) {
    if(ocelo[y][i] == stone) return 1;
    if(ocelo[y][i] == STONE_NONE) return 0;
  }

  return 0;
}

int CanPutLeft(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == 0) return 0;
  if(ocelo[y][x - 1] != enemy) return 0;

  for(i = x - 1; i >= 0; i--) {
    if(ocelo[y][i] == stone) return 1;
    if(ocelo[y][i] == STONE_NONE) return 0;
  }

  return 0;
}

int CanPutUp(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;
  
  if(y == 0) return 0;
  if(ocelo[y - 1][x] != enemy) return 0;

  for(i = y - 1; i >= 0; i--) {
    if(ocelo[i][x] == stone) return 1;
    if(ocelo[i][x] == STONE_NONE) return 0;
  }

  return 0;
}

int CanPutBelow(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(y == OCELO_HEIGHT - 1) return 0;
  if(ocelo[y + 1][x] != enemy) return 0;

  for(i = y + 1; i < OCELO_HEIGHT; i++) {
    if(ocelo[i][x] == stone) return 1;
    if(ocelo[i][x] == STONE_NONE) return 0;
  }

  return 0;
}

int CanPutUpRight(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == 0) return 0;
  if(ocelo[y - 1][x + 1] != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y - i >= 0); i++) {
    if(ocelo[y - i][x + i] == stone) return 1;
    if(ocelo[y - i][x + i] == STONE_NONE) return 0;
  }

  return 0;
}

int CanPutUpLeft(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == 0) return 0;
  if(y == 0) return 0;
  if(ocelo[y - 1][x - 1] != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y - i >= 0); i++) {
    if(ocelo[y - i][x - i] == stone) return 1;
    if(ocelo[y - i][x - i] == STONE_NONE) return 0;
  }

  return 0;
}

int CanPutBelowRight(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(ocelo[y + 1][x + 1] != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y + i < OCELO_HEIGHT); i++) {
    if(ocelo[y + i][x + i] == stone) return 1;
    if(ocelo[y + i][x + i] == STONE_NONE) return 0;
  }

  return 0;
}

int CanPutBelowLeft(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;
  
  if(x == 0) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(ocelo[y + 1][x - 1] != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y + i < OCELO_HEIGHT); i++) {
    if(ocelo[y + i][x - i] == stone) return 1;
    if(ocelo[y + i][x - i] == STONE_NONE) return 0;
  } 

  return 0;
}