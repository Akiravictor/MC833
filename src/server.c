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

#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 2048
#define TRUE 1
#define FALSE 0

void sigchld_handler(int s) {
	int saved_errno = errno;

	while( waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

int setupConnection(int max_clients, int* client_socket) {
	int sts, sockfd;
	int yes = 1;
	struct sigaction sa;
	struct addrinfo hints, *servinfo, *p;
	
	int i;

	printf("Setting up socket...\n");

	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	for (i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
	}

	if( (sts = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "GetAddrInfo Error: %s\n", gai_strerror(sts));	
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if( (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("Server: socket");
			continue;
		}

		if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
			perror("Server: setSocketOpt");
			exit(1);
		}

		if( bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if( p == NULL) {
		fprintf(stderr, "Server: failed to bind");
		exit(1);
	}

	printf("Readying listening...\n");

	if( listen(sockfd, QUEUE) == -1) {
		perror("Server: listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if( sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("Sigaction");
		exit(1);
	}
	
	return sockfd;
}

int main(){
	int in_sockfd;
	int master_socket, new_socket, client_socket[30];
	int max_clients = 30, sd, max_sd, activity;	
	int buf_size;
	int i;

	struct sockaddr_storage client_addr;
	struct sockaddr_in addr_in;
	char s[INET6_ADDRSTRLEN], buffer[MAXDATASIZE];
	socklen_t sin_size;
	pid_t process;
	fd_set readfds;

	master_socket = setupConnection(max_clients, client_socket);

	sin_size = sizeof (addr_in);

	printf("Waiting for connections...\n");

	while(TRUE) {
		FD_ZERO(&readfds);

		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		for(i = 0; i < max_clients; i++) {
			//Socket descriptor
			sd = client_socket[i];
			
			//Valid Socket should be a number diff from 0 (since we zero'ed all nodes in client)
			if(sd > 0) {
				FD_SET(sd, &readfds);
			}

			if(sd > max_sd) {
				max_sd = sd;
			}
		}
		
		//timeout is NULL
		//writefds and exceptionfds are NULL
		if( (activity = select( max_sd + 1, &readfds, NULL, NULL, NULL)) == -1 && errno != EINTR) {
			fprintf(stderr, "Server: select");
		}
		
		if( FD_ISSET(master_socket, &readfds)) {
			if( (new_socket = accept( master_socket, (struct sockaddr*) &addr_in, (socklen_t*)&sin_size)) == -1) {
				perror("Server: accept");
				exit(1);
			}
			
			printf("New connection: Socket FD: %d IP: %s PORT: %d\n", new_socket, inet_ntoa(addr_in.sin_addr), ntohs(addr_in.sin_port));

			if( send(new_socket, "Hello Client!\n", 14, 0) == -1){
				perror("Server: send");
			}

			for( i = 0; i < max_clients; i++) {
				if( client_socket[i] == 0) {
					client_socket[i] = new_socket;
					//Adds the new incoming socket to the active sockets list
					break;
				}
			}

		}

		for( i = 0; i < max_clients; i++) {
			sd = client_socket[i];

			if( FD_ISSET( sd, &readfds)){
				buf_size = recv( sd, buffer, MAXDATASIZE-1, 0);
				if( buf_size == -1) {
					perror("Server: receive");
					exit(1);
				}
				else if( buf_size == 0) {
					getpeername( sd, (struct sockaddr*)&addr_in, (socklen_t*)&sin_size);
					printf("Disconnected: IP: %s PORT: %d\n", inet_ntoa(addr_in.sin_addr), ntohs(addr_in.sin_port));

					close(sd);
					client_socket[i] = 0;
				}
				else {
					int size = buf_size;// -1;
					buffer[size] = '\0';
					printf("Received: %s from IP: %s PORT: %d\n", buffer, inet_ntoa(addr_in.sin_addr), ntohs(addr_in.sin_port));

					buffer[size] = '\n';
					buffer[size + 1] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}
	
	return 0;
} 
