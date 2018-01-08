#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include"ocelostone.h"
#include"syncgame.h"
#include"findopponent.h"

int InitSocket(char *name, char *service);
int WaitForOpponent();

FindOpponent findopponent_state;

int FindOpponentScene(char *name, char *service) {
  int retval;

  switch(findopponent_state){
    case FINDOPPONENT_INIT:
      if(InitSocket(name, service)) {
        findopponent_state = FINDOPPONENT_SEARCH;
        return;
      } else {
        exit(1);
      }
    case FINDOPPONENT_SEARCH:
      retval = WaitForOpponent();
      if(retval == 0) return;
      if(retval == -1) exit(1);
      //scene change
  }
}

int InitSocket(char *name, char *service) {
  int sockfd;
  struct addrinfo stdinfo;
  struct addrinfo *result, *rp;

  memset(&stdinfo, 0, sizeof(stdinfo));
  stdinfo.ai_family = AF_INET;
  stdinfo.ai_socktype = SOCK_STREAM;
  stdinfo.ai_protocol = 0;
  stdinfo.ai_flags = 0;

  if(getaddrinfo(name, service, &stdinfo, &result) != 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("socket");
    return 0;
  }
  
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

  return 1;
}

int WaitForOpponent() {
  fd_set readfd;
  struct timeval tval = {0, 0};
  int retval;

  FD_ZERO(&readfd);
  FD_SET(clientSockfd, &readfd);
  retval = select(clientSockfd + 1, &readfd, NULL, NULL, &tval);
  if(retval < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("socket or connect");
    close(clientSockfd);
    return -1;
  } else if(retval) {
    char buf[SYNC_BUF_SIZE];
    ssize_t size;

    size = recv(clientSockfd, buf, sizeof(buf), 0);
    if(size == 0) {
      fprintf(stderr, "server is closed\n");
      return 1;
    } else if(size < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("recv");
      close(clientSockfd);
      return -1;
    }

    if(buf[0] != SYNC_READY_NEXTTURN) {
      fprintf(stderr, "invalid header %d\n", buf[0]);
      return -1;
    }

    switch(buf[1]) {
      case 0:
        return STONE_COLOR_BLACK;
      case 1:
        return STONE_COLOR_WHITE;
    }
  }

  return 0;
}

