#include "includes.h"

int main(){
	int in_sockfd;
	int master_socket, new_socket, client_socket[30];
	int max_clients = 30, sd, max_sd, activity;	
	int buf_size;
	int i;
	
	int whoIsConnected = 0;

	struct sockaddr_storage client_addr;
	struct sockaddr_in addr_in;
	char s[INET6_ADDRSTRLEN], buffer[MAXDATASIZE];
	socklen_t sin_size;
	pid_t process;
	fd_set readfds;

	master_socket = setupConnectionS(max_clients, client_socket);

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

			/*if( send(new_socket, "Hello Client!\n", 14, 0) == -1){
				perror("Server: send");
			}*/

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
				buf_size = recvMsgS( sd, buffer);
				
				if( buf_size == 0) {
					getpeername( sd, (struct sockaddr*)&addr_in, (socklen_t*)&sin_size);
					printf("Disconnected: IP: %s PORT: %d\n", inet_ntoa(addr_in.sin_addr), ntohs(addr_in.sin_port));
					whoIsConnected = 0;
					close(sd);
					client_socket[i] = 0;
				}
				else {
					printf("Received: %s from IP: %s PORT: %d\n", buffer, inet_ntoa(addr_in.sin_addr), ntohs(addr_in.sin_port));

					if(whoIsConnected == 0 && strcmp(buffer, "login1") == 0) {
						sprintf(buffer, "connected");
						whoIsConnected = 1;
						printf("Professor conectado!\n");
					}
					else if(whoIsConnected == 0 && strcmp(buffer, "login2") == 0) {
						sprintf(buffer, "connected");
						whoIsConnected = 2;
						printf("Aluno conectado!\n");
					}
					else {
						sprintf(buffer, "nope");
					}
					
					if(whoIsConnected != 0) {
						executeMenu(buffer);
					}
					
					sendMsg(sd, buffer);
				}
			}
		}
	}
	
	return 0;
} 
