#ifndef HEADER_SYNCGAME
#define HEADER_SYNCGAME

#define SYNC_BUF_SIZE 256

typedef enum {
  SYNC_SYNCGAME = 1,
  SYNC_MYTURN,
  SYNC_OPTURN,
  SYNC_GAMEOVER,
  SYNC_SENDPOS,
  SYNC_RECVPOS,
  SYNC_FORCE_GAMEOVER
}SyncHeader;

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

extern int clientSockfd;

#endif