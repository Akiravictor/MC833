#ifndef CLIENT_H
#define CLIENT_H

#include "includes.h"

int setupConnection();
void sendMsg(int sock, char* msg);
void recvMsg(int sock, char* buffer);

#endif
