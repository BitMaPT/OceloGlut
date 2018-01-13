#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<errno.h>
#include<fcntl.h>
#include"object.h"
#include"oceloboard.h"
#include"ocelostone.h"
#include"bitstring.h"
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
int DetermineNextRoutine(char *buf);

OceloStoneColor myStoneColor;
OceloStoneColor enemyStoneColor;
GameState gameState = GAMESTATE_INIT;
//ObjectList objectList;

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
      return WaitForPut(&x, &y);
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

int SetSocket(char **argv) {
  int sockfd, retval;
  struct addrinfo stdinfo;
  struct addrinfo *result, *rp;
  
  stdinfo.ai_family = AF_INET;
  stdinfo.ai_socktype = SOCK_STREAM;
  stdinfo.ai_protocol = 0;
  stdinfo.ai_flags = 0;
  
  retval = getaddrinfo(argv[1], argv[2], &stdinfo, &result);
  if(retval != 0) {
    fprintf(stderr, "%s line:%d getaddrinfo: %s", __FILE__, __LINE__, gai_strerror(retval));
    return 0;
  }
  printf("relay");
  for(rp = result; rp != NULL; rp = rp->ai_next) {
    sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    
    if(sockfd < 0) continue;
    if(connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) break;

    close(sockfd);
  }

  if(rp == NULL) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("socket or connect");
    return 0;
  }

  freeaddrinfo(result);
  clientSockfd = sockfd;

  {
    int config;
    config = fcntl(clientSockfd, F_GETFD, 0);
    fcntl(clientSockfd, F_SETFD, config | O_NONBLOCK);
  }

  return 1;
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
    fprintf(stderr, "%s line:%d connection lost\n", __FILE__, __LINE__);
    exit(1);
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

    InitGame();
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
  double color[] = {0, 0, 0};

  if(InitBitString(0, 0, color, 200, 3, "Now Synchronizing.", "Now Synchronizing..", "Now Synchronizing...") == NULL) return 0;

  FD_ZERO(&writefds);
  FD_SET(clientSockfd, &writefds);
  retval = select(clientSockfd + 1, NULL, &writefds, NULL, &tval);

  if(retval < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("select");
    return 0;
  } else if(retval) {
    buf[0] = (char)SYNC_SYNCSIGNAL;//header for sync
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
    fprintf(stderr, "%s line:%d connection lost from server\n", __FILE__, __LINE__);
    exit(1);
    return 0;
  } else {
    //judge this is my turn and get from buf putable position
    DeleteSelectedTypeObject(OBJECT_BITSTRING);
    DetermineNextRoutine(buf);
  }

  return 1;
}

int DetermineNextRoutine(char *buf) {
  switch((SyncHeader)buf[0]) {
    case SYNC_PUTABLEPOS:
      if((OceloStoneColor)buf[1] == myStoneColor) {
        GenerateSelectablePutPoint(buf + 2);
        printf("my turn %d\n", buf[1]);
        gameState = GAMESTATE_WAIT_MYPUT;
      } else {
        printf("op turn %d\n", buf[1]);
        gameState = GAMESTATE_WAIT_OPPUT;
      }
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

    buf[0] = (char)SYNC_PUTPOSITION;//header of send position
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
    exit(1);
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
    if(MousePositionToSquarePosition(xx, yy, x, y)) {
      if(!CheckPositionIsPutable(*x, *y)) return 1;
      if(!PutStone(*x, *y, myStoneColor)) return 0;
      DeleteSelectedTypeObject(OBJECT_SELECTABLE_POINT);
      gameState = GAMESTATE_SEND_POSITION;
      return 1;
    }
  }

  return 1;
}
