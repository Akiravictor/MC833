#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>

#define ADDR "localhost"
#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 2048
#define TRUE 1
#define FALSE 0

int setupConnection() {
	int sockfd, status;
	struct addrinfo hints, *servinfo, *p;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; //sets IPv4 use
	hints.ai_socktype = SOCK_STREAM; //TCP socket type
	hints.ai_flags = AI_PASSIVE;

	if( (status = getaddrinfo(ADDR, PORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "Client: GetAddrInfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("Client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Client: connect");
			continue;
		}

		break;
	}
	
	if (p == NULL) {
		fprintf(stderr, "Client: Failed to connect\n");
		exit(1);
	}
	
	printf("Connected: Socket FD: %d IP: %s PORT: %d\n", sockfd, inet_ntoa((((struct sockaddr_in*)p->ai_addr)->sin_addr)), ntohs((((struct sockaddr_in*)p->ai_addr)->sin_port)));

	freeaddrinfo(servinfo);
	
	return sockfd;
}

int main() {
	int sockfd, buf_size;
	char buffer[MAXDATASIZE];
	char msg[MAXDATASIZE];
	int connected = TRUE;
	
	struct sockaddr_in addr_in;
	
	sockfd = setupConnection();
	
	while(connected) {
		buf_size = recv(sockfd, buffer, MAXDATASIZE-1, 0);
		if(buf_size == -1){
			perror("Client: receive");
			exit(1);
		}
		else if(buf_size == 0) {
			close(sockfd);
			connected = FALSE;
		}
		else {
			int size = buf_size -1;
			buffer[size] = '\0';
			printf("Client received %s from server\n",buffer);
		}
		printf("Message to send: ");
		fgets(msg, MAXDATASIZE, stdin);
		
		msg[strlen(msg) -1] = '\0';
		
		if(strcmp(msg,"exit") == 0){
			printf("Client: Closing\n");
			close(sockfd);
			connected = FALSE;
		}
		
		if( send(sockfd, msg, strlen(msg), 0) == -1 && connected) {
			perror("Client: send");
		}
	}
	
	
}