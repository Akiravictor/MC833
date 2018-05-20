#ifndef INCLUDE_H
#define INCLUDE_H

/* libs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

/* Custom libs */
#include "client.h"
#include "server.h"
#include "disciplinas.h"

/* defines */
#define ADDR "localhost"
#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 2048

#define TRUE 1
#define FALSE 0

#define MAXBUFLEN 100

#endif
