#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<errno.h>
#include<unistd.h>
#include"server.h"
#include"syncgame.h"
#include"ocelostone.h"
#include"serverboard.h"

typedef enum {
  SERVER_WAIT_PUT,
  SERVER_WAIT_SYNC,
  SERVER_INIT
}ServerStatus;

typedef struct PlayerTuple PlayerTuple;
struct PlayerTuple {
  int sockfds[2];
  int maxfd;
  int syncflag[2];
  int oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
  OceloStoneColor putPlayer;
  ServerStatus status;
};

typedef struct PlayerTupleList PlayerTupleList;
struct PlayerTupleList {
  PlayerTupleList *next;
  PlayerTuple *pt;
  char *name;
};

int AcceptNewConnection(int l_sockfd, struct sockaddr_in *sa);
int CreatePlayerTuple(int sockfd, char *name);
int SendGameStartSignal(PlayerTuple *pt);
int SwitchRoutineByTupleStatus(PlayerTuple *pt, int sockfd);
int CheckPutablePosition(PlayerTuple *pt);
int SendPutablePosition(PlayerTuple *pt, int *pos);
int SendGameoverSignal(PlayerTuple *pt);
int RecvSyncSignal(PlayerTuple *pt, int sockfd);
int RelayPutPosition(PlayerTuple *pt, int sockfd);
int RenewOceloBoard(PlayerTuple *pt, char *buf);
void CloseConnection(PlayerTuple *pt);
void ForceCloseConnection(PlayerTuple *pt, int sockfd);
void DeletePlayerTupleList(PlayerTuple *pt);
int DeleteInitPlayerTuple(PlayerTuple *pt, int sockfd);
void CheckLostReadySocket();


PlayerTupleList tupleHead = {NULL, NULL, NULL};
PlayerTupleList *tupleTail = &tupleHead;

PlayerTupleList readyHead = {NULL, NULL, NULL};
PlayerTupleList *readyTail = &readyHead;

#define SERVER_PORT 49152
#define BACKLOG_NUM 10

int main(int argc, char **argv) {
  int l_sockfd, maxfd, count;
  struct sockaddr_in sa;
  fd_set readfds;
  char buf[SYNC_BUF_SIZE];

  sa.sin_family = AF_INET;
  sa.sin_port = htons(SERVER_PORT);
  sa.sin_addr.s_addr = INADDR_ANY;

  l_sockfd = socket(AF_INET, SOCK_STREAM, 0);

  {
    int yes = 1;
    if(setsockopt(l_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("setsockopt");
      return 1;
    }
  }

  if(bind(l_sockfd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("bind");
    return 1;
  }

  if(listen(l_sockfd, BACKLOG_NUM) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("listen");
    return 1;
  }

  while(1) {
    PlayerTupleList *list, *before;
    PlayerTuple *pt;
    int retval, maxfd = l_sockfd;
    
    //initialize readfds for select
    FD_ZERO(&readfds);
    FD_SET(l_sockfd, &readfds);
    list = tupleHead.next;
    while(list) {
      pt = list->pt;
      if(pt->status == SERVER_INIT) {
        FD_SET(pt->sockfds[0], &readfds);
        maxfd = maxfd > pt->sockfds[0] ? maxfd : pt->sockfds[0]; 
      } else {
        FD_SET(pt->sockfds[0], &readfds);
        FD_SET(pt->sockfds[1], &readfds);
        maxfd = maxfd > pt->maxfd ? maxfd : pt->maxfd;
      } 
      
      list = list->next;
    }

    printf("select...\n");
    retval = select(maxfd + 1, &readfds, NULL, NULL, NULL);

    list = tupleHead.next;
    while(list) {
      int i;
      PlayerTuple *pt = list->pt;

      for(i = 0; i < 2; i++) {
        if(FD_ISSET(pt->sockfds[i], &readfds)) {
          //some routine for socket
          printf("ISSET %d\n", pt->sockfds[i]);
          SwitchRoutineByTupleStatus(pt, pt->sockfds[i]);
          goto loopend;
        }
      }
      list = list->next;
    }

    if(FD_ISSET(l_sockfd, &readfds)) {
      //accept new socket
      CheckLostReadySocket();
      AcceptNewConnection(l_sockfd, &sa);
    }

    loopend:;
  }
}

int AcceptNewConnection(int l_sockfd, struct sockaddr_in *sa) {
  int sockfd;
  socklen_t saSize = sizeof(struct sockaddr_in);
  char buf[SYNC_BUF_SIZE];
  ssize_t size;

  sockfd = accept(l_sockfd, (struct sockaddr*)sa, &saSize);
  if(sockfd < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("accept");
    return 0;
  }

  size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
  
  if(size < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("recv");
    return 0;
  } else if(size == 0) {
    fprintf(stderr, "%s line:%d connection lost from client\n", __FILE__, __LINE__);
    return 0;
  }

  if(!CreatePlayerTuple(sockfd, buf)) return 0;

  return 1;
}

void CheckLostReadySocket() {
  char buf[SYNC_BUF_SIZE];
  PlayerTupleList *list, *before;

  list = readyHead.next;
  before = &readyHead;
  while(list) {
    ssize_t size;
    size =recv(list->pt->sockfds[0], buf, SYNC_BUF_SIZE, MSG_DONTWAIT);
    if(size <= 0) {
      if(errno == EAGAIN || errno == EWOULDBLOCK) goto nextlist;

      close(list->pt->sockfds[0]);
      before->next = list->next;
      if(list == readyTail) {
        readyTail = list;
      }
      free(list->name);
      free(list->pt);
      free(list);
    }

    nextlist:;
    before = list;
    list = list->next;
  }
}

int CreatePlayerTuple(int sockfd, char *name) {
  PlayerTupleList *newList, *list, *before;
  PlayerTuple *tuple;
  
  //search room
  list = readyHead.next;
  before = &readyHead;
  while(list) {
    if(strcmp(list->name, name) == 0) {
      PlayerTuple *pt = list->pt;

      before->next = list->next;
      if(list == readyTail) {
        readyTail = before;
      }

      pt->sockfds[1] = sockfd;
      pt->maxfd = pt->sockfds[0] > pt->sockfds[1] ? pt->sockfds[0] : pt->sockfds[1];
      memset(pt->syncflag, 0, sizeof(pt->syncflag));
      pt->status = SERVER_WAIT_SYNC;
      Initboard(pt->oceloBoard);
      SendGameStartSignal(list->pt);

      return 1;
    }
    before = list;
    list = list->next;
  }

  //create new room
  newList = (PlayerTupleList*)malloc(sizeof(PlayerTupleList));
  if(newList == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return 0;
  }
  tuple = (PlayerTuple*)malloc(sizeof(PlayerTuple));
  if(tuple == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return 0;
  }
  newList->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
  if(newList->name == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return 0;
  }

  memset(tuple, 0, sizeof(PlayerTuple));
  tuple->status = SERVER_INIT;
  tuple->putPlayer = STONE_COLOR_BLACK;
  tuple->sockfds[0] = sockfd;

  strcpy(newList->name, name);
  newList->next = NULL;
  newList->pt = tuple;

  readyTail->next = newList;
  readyTail = newList;

  tupleTail->next = newList;
  tupleTail = newList;

  return 1;
}

int SendGameStartSignal(PlayerTuple *pt) {
  char buf[SYNC_BUF_SIZE];

  memset(buf, 0, sizeof(buf));
  buf[0] = (char)SYNC_GAMESTART;
  buf[1] = (char)STONE_COLOR_BLACK;
  if(send(pt->sockfds[0], buf, SYNC_BUF_SIZE, 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");
    
    ForceCloseConnection(pt, pt->sockfds[0]);
    return 1;
  }

  buf[1] = (char)STONE_COLOR_WHITE;
  if(send(pt->sockfds[1], buf, SYNC_BUF_SIZE, 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");

    ForceCloseConnection(pt, pt->sockfds[1]);
    return 1;
  }

  return 1;
}

int SwitchRoutineByTupleStatus(PlayerTuple *pt, int sockfd) {
  switch(pt->status) {
    case SERVER_WAIT_PUT:
      return RelayPutPosition(pt, sockfd);
    case SERVER_WAIT_SYNC:
      return RecvSyncSignal(pt, sockfd);
    case SERVER_INIT:
      return DeleteInitPlayerTuple(pt, sockfd);
  }

  return 0;
}

//if a connection of waiting opponent player's client is closed,
//this routine will be called
int DeleteInitPlayerTuple(PlayerTuple *pt, int sockfd) {
  ssize_t size;
  char buf[SYNC_BUF_SIZE];

  size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
  if(size <= 0) {
    PlayerTupleList *list, *before;

    list = readyHead.next;
    before = &readyHead;
    while(list) {
      if(list->pt == pt) {
        before->next = list->next;
        if(readyTail == list) {
          readyTail = before;
        }
        break;
      }
    }
    DeletePlayerTupleList(pt);
  } else {
    fprintf(stderr, "something bug is contained in client\n");
  }

  return 1;
}

//CheckPutablePosition.
//if there is no putable position, check opponent putable position.
//if both position is none, end game 
int CheckPutablePosition(PlayerTuple *pt) {
  int pos[OCELO_HEIGHT * OCELO_WIDTH];

  if(CheckPutablePoints(pt->oceloBoard, pos, pt->putPlayer)) {
    return SendPutablePosition(pt, pos);
  } else {
    switch(pt->putPlayer) {
      case STONE_COLOR_BLACK: 
        pt->putPlayer = STONE_COLOR_WHITE;
        break;
      case STONE_COLOR_WHITE:
        pt->putPlayer = STONE_COLOR_BLACK;
        break;
    }

    if(CheckPutablePoints(pt->oceloBoard, pos, pt->putPlayer)) {
      return SendPutablePosition(pt, pos);
    } else {
      //send gameover signal
      return SendGameoverSignal(pt);
    }
  }

  return 1;
}

//send header, put player and putable position
//put player is determined by OceloStoneColor
int SendPutablePosition(PlayerTuple *pt, int *pos) {
  char buf[SYNC_BUF_SIZE];
  int i;

  buf[0] = (char)SYNC_PUTABLEPOS;//send putable position header
  buf[1] = (char)pt->putPlayer;
  
  for(i = 0; i < OCELO_HEIGHT * OCELO_WIDTH; i++) {
    buf[i + 2] = (char)pos[i];
  }

  for(i = 0; i < 2; i++) {
    if(send(pt->sockfds[i], buf, SYNC_BUF_SIZE, 0) < 0) {
      //connection lost corresponding
      ForceCloseConnection(pt, pt->sockfds[i]);
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
      return 0;
    }
  }

  pt->status = SERVER_WAIT_PUT;

  return 1;
}

//send gameover signal
//after send signal, two players socket is closed and delete PlayerTuple from list
int SendGameoverSignal(PlayerTuple *pt) {
  int i;
  char buf[SYNC_BUF_SIZE];
  
  memset(buf, 0, sizeof(buf));
  buf[0] = (char)SYNC_GAMEOVER;//header of gameover
  for(i = 0; i < 2; i++) {
    if(send(pt->sockfds[i], buf, SYNC_BUF_SIZE, 0) < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
      return 0;
    }
  }

  CloseConnection(pt);

  return 1;
}

//recieve signals of two players
//signal means "ready to next turn"
int RecvSyncSignal(PlayerTuple *pt, int sockfd) {
  char buf[SYNC_BUF_SIZE];
  ssize_t size;

  size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
  if(size < 0) {
    ForceCloseConnection(pt, sockfd);
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("recv");
    return 0;
  } else if(size == 0) {
    ForceCloseConnection(pt, sockfd);
    return 0;
  }

  if(sockfd == pt->sockfds[0]) {
    pt->syncflag[0] = 1;
  } else {
    pt->syncflag[1] = 1;
  }

  if(pt->syncflag[0] && pt->syncflag[1]) {
    //check putable position
    pt->syncflag[0] = 0;
    pt->syncflag[1] = 0;
    return CheckPutablePosition(pt);
  }

  return 1;
}

int RelayPutPosition(PlayerTuple *pt, int sockfd) {
  char buf[SYNC_BUF_SIZE];
  int sendSockfd;
  ssize_t size;

  size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
  if(size == 0) {
    //connection lost
    ForceCloseConnection(pt, sockfd);
    return 0;
  } else if(size < 0) {
    ForceCloseConnection(pt, sockfd);
    fprintf(stderr, "%s line:%d\n", __FILE__, __LINE__);
    perror("recv");
    return 0;
  }

  if(sockfd == pt->sockfds[0]) sendSockfd = pt->sockfds[1];
  if(sockfd == pt->sockfds[1]) sendSockfd = pt->sockfds[0];

  if(send(sendSockfd, buf, SYNC_BUF_SIZE, 0) < 0) {
    ForceCloseConnection(pt, sendSockfd);
    fprintf(stderr, "%s line:%d\n", __FILE__, __LINE__);
    perror("send");
    return 0;
  }
  printf("relay\n");

  RenewOceloBoard(pt, buf);
  pt->status = SERVER_WAIT_SYNC;

  return 1;
}

int RenewOceloBoard(PlayerTuple *pt, char *buf) {
  int x, y;
  OceloStoneColor color;

  x = (int)buf[1];
  y = (int)buf[2];

  //renew ocelo board
  PutStoneOnServer(x, y, pt->putPlayer, pt->oceloBoard);

  switch(pt->putPlayer) {
    case STONE_COLOR_BLACK:
      pt->putPlayer = STONE_COLOR_WHITE;
      break;
    case STONE_COLOR_WHITE:
      pt->putPlayer = STONE_COLOR_BLACK;
      break;
  }

  return 1;
}

void CloseConnection(PlayerTuple *pt) {
  PlayerTupleList *list, *before;

  close(pt->sockfds[0]);
  close(pt->sockfds[1]);

  list = tupleHead.next;
  before = &tupleHead;

  //find pt from PlayerTuple list
  DeletePlayerTupleList(pt);
}

void ForceCloseConnection(PlayerTuple *pt, int sockfd) {
  char buf[SYNC_BUF_SIZE];
  PlayerTupleList *list, *before;

  memset(buf, 0, sizeof(buf));

  buf[0] = SYNC_GAMEOVER_FORCE;
  if(sockfd == pt->sockfds[0]) {
    printf("send1\n");
    if(send(pt->sockfds[1], buf, SYNC_BUF_SIZE, 0) < 0){
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
    }
  }
  if(sockfd == pt->sockfds[1]) {
    printf("send2\n");
    if(send(pt->sockfds[0], buf, SYNC_BUF_SIZE, 0) < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
    }
  }

  // shutdown(pt->sockfds[0], SHUT_WR);
  // shutdown(pt->sockfds[1], SHUT_WR);
  close(pt->sockfds[0]);
  close(pt->sockfds[1]);

  //find pt from PlayerTuple list
  DeletePlayerTupleList(pt);
}

void DeletePlayerTupleList(PlayerTuple *pt) {
  PlayerTupleList *list, *before;

  list = tupleHead.next;
  before = &tupleHead;
  while(list) {
    if(list->pt == pt) {
      before->next = list->next;
      if(list == tupleTail) {
        tupleTail = before;
      }

      free(list->pt);
      free(list->name);
      free(list);

      return;
    }

    before = list;
    list = list->next;
  }
}