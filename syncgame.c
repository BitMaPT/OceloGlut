#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include"syncgame.h"

void StoreIntHtoN(char *buf, int num);
void GetIntNtoH(char *buf, int *num);

int clientSockfd = 0;