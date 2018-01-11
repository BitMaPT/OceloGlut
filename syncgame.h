#ifndef HEADER_SYNCGAME
#define HEADER_SYNCGAME

#define SYNC_BUF_SIZE 256

typedef enum {
  SYNC_GAMESTART,
  SYNC_PUTABLEPOS,
  SYNC_PUTPOSITION,
  SYNC_GAMEOVER,
  SYNC_GAMEOVER_FORCE
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