#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include"object.h"
#include"oceloboard.h"
#include"ocelostone.h"
#include"gamecontroller.h"
#include"mouse.h"
#include"syncgame.h"

int RecvMyStoneColor();
int SendSignalForSync();
int WaitForPut(int *x, int *y);
int GetPutablePosition();
int RecvPutPosition();
int SendPutPosition(int x, int y);
int GameOver();

OceloStoneColor myStoneColor;
OceloStoneColor enemyStoneColor;
GameState gameState;
ObjectList objectList;

int ControlGameWithState() {
  static int x, y;

  switch(gameState) {
    case GAMESTATE_INIT:
      //wait for my color
      return RecvMyStoneColor();
    case GAMESTATE_SEND_SIGNAL:
      //send signal for sync
      return SendSignalForSync();
    case GAMESTATE_GET_PUTABLE:
      //wait for sending putable position info
      return GetPutablePosition();
    case GAMESTATE_WAIT_MYPUT:
      //wait for my puting stone
      if(WaitForPut(&x, &y)) {
        gameState = GAMESTATE_SEND_POSITION;
      }
      return 1;
    case GAMESTATE_WAIT_OPPUT:
      //recieve wait for opponent putting
      return RecvPutPosition();
    case GAMESTATE_SEND_POSITION:
      //send put position
      return SendPutPosition(x, y);
    case GAMESTATE_REVERSE:
      if(CheckAllStoneReversed()) {
        gameState = GAMESTATE_SEND_SIGNAL;
      }
      return 1;
  }

  return 0;
}

void InitGame() {
  
  int x, y;

  DeleteAllObject();

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      oceloBoard[y][x] = NULL;
    }
  }

  oceloBoard[3][3] = InitOceloStone(3, 3, STONE_COLOR_BLACK);
  oceloBoard[3][4] = InitOceloStone(4, 3, STONE_COLOR_WHITE);
  oceloBoard[4][3] = InitOceloStone(3, 4, STONE_COLOR_WHITE);
  oceloBoard[4][4] = InitOceloStone(4, 4, STONE_COLOR_BLACK);

  return;
}

int RecvMyStoneColor() {
  ssize_t size;
  char buf[SYNC_BUF_SIZE];

  size = recv(clientSockfd, buf, SYNC_BUF_SIZE, 0);
  if(size < 0) {
    if(!(errno == EAGAIN || errno == EWOULDBLOCK)) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("recv");
      return 0;
    } 
  } else if(size == 0) {
    //TODO
    //connection lost
    return 0;
  } else {
    //receive data from server
    switch((OceloStoneColor)buf[1]) {
      case STONE_COLOR_BLACK:
        myStoneColor = STONE_COLOR_BLACK;
        enemyStoneColor = STONE_COLOR_WHITE;
        break;
      case STONE_COLOR_WHITE:
        myStoneColor = STONE_COLOR_WHITE;
        enemyStoneColor = STONE_COLOR_BLACK;
        break;
    }

    gameState = GAMESTATE_SEND_SIGNAL;
  }

  return 1;
}

int SendSignalForSync() {
  ssize_t size;
  fd_set writefds;
  char buf[SYNC_BUF_SIZE];
  int retval;
  struct timeval tval = {0, 0};

  FD_ZERO(&writefds);
  FD_SET(clientSockfd, &writefds);
  retval = select(clientSockfd + 1, NULL, &writefds, NULL, &tval);

  if(retval < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("select");
    return 0;
  } else if(retval) {
    buf[0] = (char)SYNC_SYNCGAME;//header for sync
    size = send(clientSockfd, buf, SYNC_BUF_SIZE, 0);
    if(size < 0) {
      if(!(errno == EAGAIN || errno == EWOULDBLOCK)) {
        fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
        perror("send");
        return 0;
      }

      return 1;
    }

    gameState = GAMESTATE_GET_PUTABLE;
    return 1;
  }

  return 1;
}

int GetPutablePosition() {
  ssize_t size;
  char buf[SYNC_BUF_SIZE];

  size = recv(clientSockfd, buf, SYNC_BUF_SIZE, 0);
  if(size < 0) {
    if(!(errno == EAGAIN || errno == EWOULDBLOCK)) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("recv");
      return 0;
    }
    return 1;
  } else if(size == 0) {
    //TODO: connection lost
    return 0;
  } else {
    //judge this is my turn and get from buf putable position
    if(buf[1] == myStoneColor) {
      gameState = GAMESTATE_WAIT_MYPUT;
    } else {
      gameState = GAMESTATE_WAIT_OPPUT;
    }
    GenerateSelectablePutPoint(buf + 2);
  }

  return 1;
}

int DetermineNextRoutine(char *buf) {
  switch((SyncHeader)buf[1]) {
    case SYNC_MYTURN:
      gameState = GAMESTATE_WAIT_MYPUT;
      return 1;
    case SYNC_OPTURN:
      gameState = GAMESTATE_WAIT_OPPUT;
      return 1;
    case SYNC_GAMEOVER:
      GameOver();
      return 1;
    default:
      return 0;
  }
}

int GameOver() {
  close(clientSockfd);
  exit(0);
}

int SendPutPosition(int x, int y) {
  int retval;
  struct timeval tval = {0, 0};
  fd_set writefds;
  
  FD_ZERO(&writefds);
  FD_SET(clientSockfd, &writefds);
  retval = select(clientSockfd + 1, NULL, &writefds, NULL, &tval);
  
  if(retval < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("select");
    return 0;
  } else if(retval) {
    char buf[SYNC_BUF_SIZE];

    buf[0] = SYNC_SENDPOS;//header of send position
    buf[1] = (char)x;
    buf[2] = (char)y;
    if(send(clientSockfd, buf, SYNC_BUF_SIZE, 0) < 0) {
      if(!(errno == EAGAIN || errno == EWOULDBLOCK)) {
        fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
        perror("send");
        return 0;
      }
      return 1;
    }

    gameState = GAMESTATE_REVERSE;
  }

  return 1;
}

int RecvPutPosition() {
  char buf[SYNC_BUF_SIZE];
  ssize_t size;

  size = recv(clientSockfd, buf, SYNC_BUF_SIZE, 0);
  if(size < 0) {
    if(!(errno == EAGAIN || errno == EWOULDBLOCK)) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("recv");
      return 0;
    }
    return 1;
  } else if(size == 0) {
    //TODO:
    //connection lost
  } else {
    int x, y;
    
    x = (int)buf[1];
    y = (int)buf[2];
    PutStone(x, y, enemyStoneColor);
    gameState = GAMESTATE_REVERSE;
    return 1;
  }
}

int WaitForPut(int *x, int *y) {
  int xx, yy;
  
  if(GetMouseDown(GLUT_LEFT_BUTTON, &xx, &yy)) {
    if(MousePositionToSquarePosition(xx, yy, x, y)) return 1;
  }

  return 0;
}
