#ifndef HEADER_SYNCGAME
#define HEADER_SYNCGAME

#define SYNC_BUF_SIZE 32

typedef enum {
  SYNC_OPPONENT_LOST = 1,
  SYNC_READY_NEXTTURN,
  SYNC_GAMESTART,
  SYNC_POSITION,
  SYNC_WAITPUT,
  SYNC_NO_POSITION,
  SYNC_ENDGAME,
  SYNC_INITGAME_READY
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