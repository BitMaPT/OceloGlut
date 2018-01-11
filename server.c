#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<unistd.h>
#include"server.h"
#include"syncgame.h"
#include"ocelostone.h"
#include"oceloboard.h"

typedef enum {
  SERVER_WAIT_SYNC,
  SERVER_WAIT_CHECKPOS,
  SERVER_WAIT_PUT
}ServerStatus;

typedef struct PlayerTuple PlayerTuple;
struct PlayerTuple {
  int sockfds[2];
  int maxfd;
  int syncflag[2];
  int oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
  OceloStoneColor putPlayer;
  ServerStatus status;
  PlayerTuple *next;
};

PlayerTuple tupleHead;
PlayerTuple *tupleTail = &tupleHead;

#define SERVER_PORT 49152
#define BACKLOG_NUM 10

int WaitForTwoPlayers(int l_sockfd, struct sockaddr_in sa);
int WaitForSignalFromClient(char *buf);
int WaitForSync();
int RecvSignalFromClient(int sockfd, char *buf);

int sockfds[2];
static ServerStatus status = SERVER_WAIT_SYNC;

int main(int argc, char **argv) {
  int l_sockfd, maxfd, count;
  struct sockaddr_in sa;
  fd_set readfds;
  char buf[SYNC_BUF_SIZE];

  sa.sin_family = AF_INET;
  sa.sin_port = htons(SERVER_PORT);
  sa.sin_addr.s_addr = INADDR_ANY;

  l_sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
    PlayerTuple *tuple;
    int retval, maxfd = l_sockfd;
    
    //initialize readfds for select
    FD_ZERO(&readfds);
    FD_SET(l_sockfd, &readfds);
    tuple = tupleHead.next;
    while(tuple) {
      FD_SET(tuple->sockfds[0], &readfds);
      FD_SET(tuple->sockfds[1], &readfds);
      maxfd = maxfd > tuple->maxfd ? maxfd : tuple->maxfd;
      tuple = tuple->next;
    }

    retval = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if(FD_ISSET(l_sockfd, &readfds)) {
      //accept new socket
      continue;
    }
    tuple = tupleHead.next;
    while(tuple) {
      if(FD_ISSET(tuple->sockfds[0], &readfds)) {
        //some routine for socket
        break;
      }
      if(FD_ISSET(tuple->sockfds[1], &readfds)) {
        //some routine for socket
        break;
      }
      tuple = tuple->next;
    }
  }
}

int AcceptNewConnection(int l_sockfd, struct sockaddr_in *sa) {
  int sockfd;
  socklen_t saSize = sizeof(struct sockaddr_in);

  sockfd = accept(l_sockfd, (struct sockaddr*)sa, &saSize);
  if(sockfd < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("accept");
    return 0;
  }

  if(!CreatePlayerTuple(sockfd)) return 0;

  return 1;
}

int CreatePlayerTuple(int sockfd) {
  static PlayerTuple tuple;
  static int isInit = 1;
  PlayerTuple *pt;

  if(isInit) {
    memset(&tuple, 0, sizeof(PlayerTuple));
    tuple.sockfds[0] = sockfd;
    isInit = 0;
  } else {
    tuple.sockfds[1] = sockfd;
    tuple.status = ;
    tuple.next = NULL;
    tuple.putPlayer = STONE_COLOR_BLACK;

    pt = (PlayerTuple*)malloc(sizeof(PlayerTuple));
    if(pt == NULL) {
      fprintf(stderr, "%s line:%d malloc error", __FILE__, __LINE__);
      return 0;
    }

    memcpy(pt, &tuple, sizeof(PlayerTuple));
    
    tupleTail->next = pt;
    tupleTail = pt;
    
    //determine and send player stone colors to players

    isInit = 1;
  }

  return 1;
}

int SwitchRoutineByTupleStatus(PlayerTuple *pt, int sockfd) {
  switch(pt->status) {
    //switch by status
  }
}

//CheckPutablePosition.
//if there is no putable position, check opponent putable position.
//if both position is none, end game 
int CheckPutablePosition(PlayerTuple *pt) {
  if(/*check putable position*/) {
    if(!SendPutablePosition(pt)) return 0;
  } else {
    switch(pt->putPlayer) {
      case STONE_COLOR_BLACK: 
        pt->putPlayer = STONE_COLOR_WHITE;
        break;
      case STONE_COLOR_WHITE:
        pt->putPlayer = STONE_COLOR_BLACK;
        break;
    }
    if(/*check putable position*/) {
      if(!SendPutablePosition(pt)) return 0;
    } else {
      //send gameover signal
    }
  }

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

//send header, put player and putable position
//put player is determined by OceloStoneColor
int SendPutablePosition(PlayerTuple *pt, int *pos, int size) {
  char buf[SYNC_BUF_SIZE];
  int i;

  buf[0] = ;//send putable position header
  buf[1] = (char)pt->putPlayer;
  
  for(i = 0; i < size; i++) {
    buf[i + 2] = (char)pos[i];
  }

  for(i = 0; i < 2; i++) {
    if(send(pt->sockfds[i], buf, SYNC_BUF_SIZE, 0) < 0) {
      //TODO connection lost corresponding
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
      //delete playerTuple list
      return 0;
    }
  }

  return 1;
}

//send gameover signal
//after send signal, two players socket is closed and delete PlayerTuple from list
int SendGameoverSignal(PlayerTuple *pt) {
  int i;
  char buf[SYNC_BUF_SIZE];
  
  buf[0] = ;//header of gameover
  for(i = 0; i < 2; i++) {
    if(send(pt->sockfds[i], buf, SYNC_BUF_SIZE, 0) < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
      return 0;
    }
  }

  return 1;
}

int RecvSyncSignal(PlayerTuple *pt, int sockfd) {
  char buf[SYNC_BUF_SIZE];
  ssize_t size;

  size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
  if(size < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("recv");
    return 0;
  }

  if(sockfd == pt->sockfds[0]) {
    pt->syncflag[0] = 1;
  } else {
    pt->syncflag[1] = 1;
  }

  

  return 1;
}

int SendSignalToAllClient(SyncHeader header) {
  char buf[SYNC_BUF_SIZE];
  int i;

  buf[0] = (char)header;
  for(i = 0; i < 2; i++) {
    if(send(sockfds[i], buf, SYNC_BUF_SIZE, 0) < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
      return 0;
    }
  }
  
  return 1;
}

int SendPutPosition(int sockfd, int x, int y) {
  char buf[SYNC_BUF_SIZE];

  buf[0] = ; //store header of put position
  buf[1] = (char)x;
  buf[2] = (char)y;

  if(send(sockfd, buf, SYNC_BUF_SIZE, 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");
    return 0;
  }

  return 1;
}

int RecvPutPosition(int sockfd) {
  char buf[SYNC_BUF_SIZE];
  int sendSockfd;
  ssize_t size;
  
  size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
  if(size == 0) {
    //connection lost
  } else if(size < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("recv");
    return 0;
  }

  sendSockfd = (sockfd == sockfds[0]) ? sockfds[0] : sockfds[1];
  if(!SendPutPosition(sendSockfd, buf[1], buf[2])) return 0;
  
  return 1;
}

int SendPutablePosition(int sockfd, int *list, size_t size) {
  char buf[SYNC_BUF_SIZE];
  int i;

  buf[0] = ;//store header of put position info
  for(i = 0; i < size; i++) {
    buf[i + 1] = (char)list[i];
  }

  if(send(sockfd, buf, SYNC_BUF_SIZE, 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");
    return 0;
  }

  return 1;
}

//recieve syncronise signal from two clients
int RecvSyncSignal() {
  int i, j, retval, maxfd;
  int sockfd;
  fd_set readfds;

  FD_ZERO(&readfds);
  for(i = 0; i < 2; i++) {
    FD_SET(sockfds[i], &readfds);
  }

  maxfd = (sockfds[0] > sockfds[1]) ? sockfds[0] : sockfds[1];
  retval = select(maxfd + 1, &readfds, NULL, NULL, NULL);
  
  if(retval <= 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("select");
    return 0;
  } else {
    //recive signal from client
    for(i = 0; i < 2; i++) {
      if(FD_ISSET(sockfds[i], &readfds)) {
        ssize_t size;
        char buf[SYNC_BUF_SIZE];

        //not selected socket file discripter is stored to sockfd
        switch(i) {
          case 0: sockfd = sockfds[1]; break;
          case 1: sockfd = sockfds[0]; break;
        }

        size = recv(sockfds[i], buf, SYNC_BUF_SIZE, 0);
        if(size == 0) {
          //connection lost
          return 0;
        } else if(size < 0) {
          fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
          perror("recv");
          return 0;
        }

        size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
        if(size == 0) {
          //connection lost
          return 0;
        } else if(size < 0) {
          fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
          perror("recv");
          return 0;
        }
        
        break;
      }
    }
  }

  return 1;
}