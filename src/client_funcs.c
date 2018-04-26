#include "includes.h"

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

void sendMsg(int sock, char* msg) {
	if( send(sock, msg, strlen(msg), 0) == -1) {
		perror("Client: send");
		exit(1);
	}
}

void recvMsg(int sock, char* buffer) {
	int buf_size;
	
	buffer[0] = '\0';
	
	buf_size = recv(sock, buffer, MAXDATASIZE -1, 0);
	
	if( buf_size == -1) {
		perror("Client: receive");
		exit(1);
	}
	else if( buf_size == 0) {
		close(sock);
	}
	else {
		//int size = buf_size -1;
		buffer[buf_size] = '\0';
	}
}