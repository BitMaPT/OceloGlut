#ifndef HEADER_SYNCGAME
#define HEADER_SYNCGAME

int SendGameEnd();
int SendNoPositionToPut();
int SendPutPoint(int x, int y);
int RecvPutPoint(int *x, int *y);
int RecvStartGame();

#endif