#ifndef _TCP_H_
#define _TCP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int CreateSever(int);
int AcceptSocket(int);
int ConnectToSever(char *, int);
int ReadSocket(int, unsigned char *, int);
int WriteSocket(int, unsigned char *, int);
void CloseSocket(int);
#endif
