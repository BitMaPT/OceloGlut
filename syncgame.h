#ifndef HEADER_SYNCGAME
#define HEADER_SYNCGAME

int SendGameEnd();
int SendNoPositionToPut();

//retval:
//0=> error occured
//1=> there is no problem
int SendPutPoint(int x, int y);
int RecvPutPoint(int *x, int *y);
int RecvStartGame();

#endif