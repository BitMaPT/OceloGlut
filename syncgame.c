#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include"syncheader.h"

void StoreIntHtoN(char *buf, int num);
void GetIntNtoH(char *buf, int *num);

int clientSockfd;

int SendNoPositionToPut() {
  char buf[SYNC_BUF_SIZE];

  buf[0] = (char)SYNC_NO_POSITION;
  if(send(clientSockfd, buf, sizeof(buf), 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");
    return 0;
  }

  return 1;
}

int SendGameEnd() {
  char buf[SYNC_BUF_SIZE];

  buf[0] = (char)SYNC_ENDGAME;
  if(send(clientSockfd, buf, sizeof(buf), 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");
    return 0;
  }

  return 1;
}

//retval:
//0=> error occured
//1=> there is no problem
int SendPutPoint(int x, int y) {
  char buf[SYNC_BUF_SIZE];

  buf[0] = 1;
  StoreIntHtoN(buf + 1, x);
  StoreIntHtoN(buf + 5, y);
  
  if(send(clientSockfd, buf, sizeof(buf), 0) < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("send");
    return 0;
  }

  return 1;
}

int RecvPutPoint(int *x, int *y) {
  fd_set readfd;
  struct timeval tval = {0, 0};
  int retval;

  FD_ZERO(&readfd);
  FD_SET(clientSockfd, &readfd);
  
  retval = select(clientSockfd + 1, &readfd, NULL, NULL, &tval);
  if(retval < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("select");
    return -1;
  } else if(retval) {
    char buf[SYNC_BUF_SIZE];
    ssize_t size;

    size = recv(clientSockfd, buf, sizeof(buf), 0);
    if(size < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("recv");
      return -1;
    } else if(size == 0) {
      fprintf(stderr, "%s line:%d server is closed\n", __FILE__, __LINE__);
      return -1;
    }

    switch(buf[0]) {
      case 0:
        //opponent connection is closed
      case 2:
        GetIntNtoH(buf + 1, x);
        GetIntNtoH(buf + 5, y);
        return 1;
    }
  }

  return 0;
}

int RecvStartGame() {
  fd_set readfd;
  struct timeval tval = {0, 0};
  int retval;

  FD_ZERO(&readfd);
  FD_SET(clientSockfd, &readfd);
  retval = select(clientSockfd + 1, &readfd, NULL, NULL, &tval);
  if(retval < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("recv");
    return -1;
  } else if(retval) {
    ssize_t size;
    char buf[SYNC_BUF_SIZE];
    size = recv(clientSockfd, buf, sizeof(buf), 0);
    
    if(size < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("recv");
      close(clientSockfd);
      return -1;
    } else if(size == 0) {
      fprintf(stderr, "%s, line:%d server is closed\n", __FILE__, __LINE__);
      close(clientSockfd);
      return -1;
    }

    switch(buf[0]) {
      case (char)SYNC_GAMESTART:
        return 1;
      default:
        fprintf(stderr, "%s line:%d invalid header type(%d)\n", __FILE__, __LINE__, buf[0]);
        return -1;
    }
  }

  return 0;
}

void StoreIntHtoN(char *buf, int num) {
  int netnum, i;

  for(i = 0; i < 4; i++) {
    buf[i] = 0;
  }

  netnum = htonl(num);

  buf[0] |= (netnum >> 0);
  buf[1] |= (netnum >> 8);
  buf[2] |= (netnum >> 16);
  buf[3] |= (netnum >> 24);
}

void GetIntNtoH(char *buf, int *num) {
  int netnum = 0;

  netnum |= ((int)buf[0] << 0);
  netnum |= ((int)buf[1] << 8);
  netnum |= ((int)buf[2] << 16);
  netnum |= ((int)buf[3] << 24);

  *num = ntohl(netnum);
}