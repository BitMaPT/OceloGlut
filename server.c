#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include"server.h"
#include"syncgame.h"
#include"ocelostone.h"

typedef enum {
  SERVER_WAIT_SYNC,
  SERVER_WAIT_CHECKPOS,
  SERVER_WAIT_PUT
}ServerStatus;

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

  while(1) {
    count = 0;
    if(WaitForTwoPlayers(l_sockfd, sa)) exit(1);

    maxfd = sockfds[0] > sockfds[1] ? sockfds[0] : sockfds[1];

    buf[0] = SYNC_GAMESTART;
    buf[1] = STONE_COLOR_BLACK;
    if(send(sockfds[0], buf, sizeof(buf), 0) < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
      return 1;
    }

    buf[1] = STONE_COLOR_WHITE;
    if(send(sockfds[1], buf, sizeof(buf), 0) < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("send");
      return 1;
    }

    while(1) {

    }
  }
}

int ServerControlState() {
  switch(status) {
    case SERVER_WAIT_SYNC:
      WaitForSync();
    case SERVER_WAIT_CHECKPOS:
    case SERVER_WAIT_PUT:
  }
}

int WaitForSync() {
  char *buf;
  int p1, p2;

  p1 = 0;
  p2 = 0;

  buf = (char*)malloc(sizeof(char) * SYNC_BUF_SIZE);

  while(p1 && p2) {
    if(WaitForSignalFromClient(buf) == 0) {
      exit(1);
    }

    if(buf[0] == SYNC_READY_NEXTTURN) {
      switch(buf[1]) {
        case STONE_COLOR_BLACK: 
          p1 = 1;
          break;
        case STONE_COLOR_WHITE:
          p2 = 1;
          break;
        default:
          fprintf(stderr, "%s, line:%d invalid data\n", __FILE__, __LINE__);
          return 0;
      }
    } else {
      fprintf(stderr, "%s line:%d invalid header\n", __FILE__, __LINE__);
      return 0;
    }
  }

  free(buf);

  return 1;
}

int WaitCheckPos() {
  SyncHeader headers[2], *header;
  char *buf;
  int p1 = 0, p2 = 0;

  buf = (char*)malloc(sizeof(char) * SYNC_BUF_SIZE);

  while(1) {
    WaitForSignalFromClient(buf);

    switch(buf[1]) {
      case STONE_COLOR_BLACK:
        header = &headers[1];
        p1 = 1;
        break;
      case STONE_COLOR_WHITE:
        header = &headers[0];
        p2 = 1;
        break;
    }

    switch(buf[0]) {
      case SYNC_WAITPUT:
        *header = SYNC_WAITPUT;
        break;
      case SYNC_NO_POSITION:
        *header = SYNC_NO_POSITION;
        break;
      case SYNC_ENDGAME:
        *header = SYNC_ENDGAME;
        break;
    }

    if(p1 && p2) {
      if(headers[0] == headers[1]) {
        if(send(sockfds[0], buf, SYNC_BUF_SIZE, 0) < 0) {
          fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
          perror("send");
          return 0;
        }
        if(send(sockfds[1], buf, SYNC_BUF_SIZE, 0) < 0) {
          fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
          perror("send");
          return 0;
        }

        if(headers[0] == SYNC_WAITPUT) {
          status = SERVER_WAIT_PUT;
        } else {
          status = SERVER_WAIT_SYNC;
        }
        return 1;
      }
    } else {
      buf[0] = SYNC_CHECK_MISMATCH;
      if(send(sockfds[0], buf, SYNC_BUF_SIZE, 0) < 0) {
        fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
        perror("send");
        return 0;
      }
      if(send(sockfds[1], buf, SYNC_BUF_SIZE, 0) < 0) {
        fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
        perror("send");
        return 0;
      }

      return 0;
    }
  }
  
}

int WaitPutPos() {
  char *buf;

  buf = (char*)malloc(sizeof(char) * SYNC_BUF_SIZE);

  if(WaitForSignalFromClient(buf) == 0){
    free(buf);
    return 0;
  }

  if(buf[0] == SYNC_POSITION) {
    switch(buf[1]) {
      case STONE_COLOR_BLACK:
        if(send(sockfds[1], buf, SYNC_BUF_SIZE, 0) < 0) {
          fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
          perror("send");
          return 0;
        }
        break;
      case STONE_COLOR_WHITE:
        if(send(sockfds[0], buf, SYNC_BUF_SIZE, 0) < 0) {
          fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
          perror("send");
          return 0;
        }
        break;
      default:
        fprintf(stderr, "%s line:%d error stonecolor\n", __FILE__, __LINE__);
        return 0;
    }
    return 1;
  }

  return 0;
}

int WaitForTwoPlayers(int l_sockfd, struct sockaddr_in sa) {
  struct timeval tval = {0, 0};
  fd_set readfds;
  int retval, count = 0;
  char buf[1];

  while(count < 2) {
    FD_ZERO(&readfds);
    FD_SET(l_sockfd, &readfds);
    
    retval = select(l_sockfd + 1, &readfds, NULL, NULL, NULL);
    
    if(retval < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("select");
      return 0;

      //accept is ready
    } else if (retval) {
      int new_sockfd;
      socklen_t size;
      ssize_t recv_size;

      size = sizeof(sa);
      new_sockfd = accept(l_sockfd, (struct sockaddr*)&sa, &size);
      if(new_sockfd < 0) {
        fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
        perror("accept");
        return 0;
      }

      sockfds[count++] = new_sockfd;
    }
  }

  return 1;
}

int WaitForSignalFromClient(char *buf) {
  int retval, maxfd;
  fd_set readfds;

  FD_ZERO(&readfds);
  FD_SET(sockfds[0], &readfds);
  FD_SET(sockfds[1], &readfds);

  maxfd = (sockfds[0] > sockfds[1]) ? sockfds[0] : sockfds[1];
  retval = select(maxfd + 1, &readfds, NULL, NULL, NULL);

  if(retval < 0) {
    fprintf(stderr, "%s, line:%d ", __FILE__, __LINE__);
    perror("select");
    close(sockfds[0]);
    close(sockfds[1]);
    return 0;
  } else if(retval) {
    if(FD_ISSET(sockfds[0], &readfds)) {
      RecvSignalFromClient(sockfds[0], buf);
      return 1;
    }
    if(FD_ISSET(sockfds[1], &readfds)) {
      RecvSignalFromClient(sockfds[1], buf);
      return 1;
    }

    return 0;
  }

  return 1;
}

int RecvSignalFromClient(int sockfd, char *buf) {
  ssize_t size;

  size = recv(sockfd, buf, SYNC_BUF_SIZE, 0);
  if(size == 0) return 0;
  if(size <  0) {
    close(sockfds[0]);
    close(sockfds[1]);
    return -1;
  }

  return 1;
}

int SendSignalToClient(int sockfd, SyncHeader header) {
  ssize_t size;
  char buf[SYNC_BUF_SIZE];

  buf[0] = header;
  if(send(sockfd, buf, sizeof(buf), 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");
    return 0;
  }

  return 1;
}