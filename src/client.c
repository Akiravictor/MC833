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

void sendMsg(int sock, char* msg, int msgLen){
	if( send(sockfd, a, strlen(login), 0) == -1) {
		perror("Client: send");
	}
}

char* recvMsg(int sock, int* connected) {
	char buffer[MAXDATASIZE];
	int buf_size;
	
	buf_size = recv(sock, buffer, MAXDATASIZE -1, 0);
	
	if( buf_size == -1) {
		perror("Server: receive");
		exit(1);
	}
	else if( buf_size == 0) {
		close(sock);
		connected = FALSE;
	}
	else {
		//int size = buf_size -1;
		buffer[buf_size] = '\0';
	}
	
	return buffer;
}

int main() {
	int sockfd, buf_size;
	char buffer[MAXDATASIZE];
	char msg[MAXDATASIZE];
	int persConn = 0;
	int op = 0;
	int connected = FALSE;
	char profMenu[] = "Menu Principal:\n 1 - Lista de Disciplinas\n 2 - Ementa de uma Disciplina\n 3 - Informacoes de uma Disciplina\n 4 - Lista de informacoes de todas as disciplinas\n 5 - Adicionar uma mensagem em uma disciplina\n 6 - Mensagens de uma disciplina\n 7 - Alterar ementa de uma disciplina\n 8 - Alterar mensagem de uma disciplina\n 9 - Alterar sala de uma disciplina\n10 - Alterar horario de uma disciplina\n11 - Remover uma disciplina\n12 - Adicionar uma disciplina\n13 - Sair\n\n";
	char studMenu[] = "Menu Principal:\n 1 - Lista de Disciplinas\n 2 - Ementa de uma Disciplina\n 3 - Informacoes de uma Disciplina\n 4 - Lista de informacoes de todas as disciplinas\n 5 - Mensagens de uma disciplina\n 7 - Sair\n\n";
	
	struct sockaddr_in addr_in;
	
	sockfd = setupConnection();
	
	printf("Quem eh voce?\n1 - Professor\n2 - Aluno\n3 - Sair\n");
	printf("Opcao: ");
	scanf("%d",&persConn);
	
	do {
		int n;
		char login[6];
		n = sprintf(a, "login%d", persConn);
		
		switch(persConn){
			case 1:		
				sendMsg(sockfd, login, strlen(login));
				
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
				
				break;
			case 2:
				
				if( send(sockfd, a, 1, 0) == -1) {
					perror("Client: send");
				}
				
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
				break;
			
			case 3:
				break;
			
			default:
				printf("Opcao inexistente!\n");
				break;
		}
		
	} while(!connected && persConn != 3);
	
	if((persConn == 1 && op == 13) || (persConn == 2 && op == 7) || persConn == 3) {
		printf("Saindo...\n");
		connected = FALSE;
		close(sockfd);
	}
	
	while(connected) {
		printf("Uai....\n");
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
		//fgets(msg, MAXDATASIZE, stdin);
		
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