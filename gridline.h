#ifndef HEADER_GLIDLINE
#define HEADER_GLIDLINE

typedef struct {
  int pos[2]; //position of upleft
  int pointSize;
  int lineWidth;
}GridLine;

GridLine* InitGridLine(int pos[2], int line, int point);
#endif