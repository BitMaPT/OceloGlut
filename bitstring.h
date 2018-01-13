#ifndef HEADER_BITSTRING
#define HEADER_BITSTRING

typedef struct {
  int startTime;
  int stateTime;
  int shiftTime;
  int stateNum;
}StringState;

typedef struct {
  char **strPattern;
  int patternNum;
  int pos[2];
  double color[3];
  StringState state;
}BitString;

BitString* InitBitString(int x, int y, double color[3], int ctime, int patterns, ...);

#endif