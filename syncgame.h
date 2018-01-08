#ifndef HEADER_SYNCGAME
#define HEADER_SYNCGAME

#include"syncheader.h"

int SendGameEnd();
int SendNoPositionToPut();

//retval:
//0=> error occured
//1=> there is no problem
int SendPutPoint(int x, int y);
int RecvPutPoint(int *x, int *y);
int RecvStartGame();
int RecvSignalFromServer();
int SendSignalToServer(SyncHeader header);

#endif