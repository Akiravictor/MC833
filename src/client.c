#include "includes.h"

#define PORT "54321"
#define ADDR "localhost"
#define MAXDATASIZE 2048

int main(){
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	char msg[MAXDATASIZE];
	struct addrinfo hints, *serverInfo, *p;
	int status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; //sets IPv4 use
	hints.ai_socktype = SOCK_STREAM; //TCP socket type
	hints.ai_flags = AI_PASSIVE;

	if( (status = getaddrinfo(ADDR, PORT, &hints, &serverInfo)) != 0){
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	// loop through all the results and connect to the first we can
	for(p = serverInfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	
	freeaddrinfo(serverInfo);
	
	while(1){
		if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		
		if(numbytes == 0){
			close(sockfd);
			exit(0);
		}

		buf[numbytes-1] = '\0';

		printf("client: received '%s'\n",buf);
		
		printf("digite a mensage: ");
		fgets(msg, MAXDATASIZE,stdin);
		
		//msg[strlen(msg) - 1] = '\0';
		
		if (send(sockfd, msg, strlen(msg), 0) == -1)
			perror("send");

		buf[numbytes] = '\0';

		printf("client: received '%s'\n",buf);	
		
	}

	return 0;
}
