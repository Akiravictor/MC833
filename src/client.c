#include "includes.h"

int main() {
	int sockfd, buf_size;
	char buffer[MAXDATASIZE];
	char msg[MAXDATASIZE];
	int persConn = 0;
	int op = 0;
	int connected = FALSE;
	char profMenu[] = "Menu Principal:\n 1 - Lista de Disciplinas\n 2 - Ementa de uma Disciplina\n 3 - Informacoes de uma Disciplina\n 4 - Lista de informacoes de todas as disciplinas\n 5 - Mensagens de uma disciplina\n 6 - Adicionar uma mensagem em uma disciplina\n 7 - Alterar ementa de uma disciplina\n 8 - Alterar mensagem de uma disciplina\n 9 - Alterar sala de uma disciplina\n10 - Alterar horario de uma disciplina\n11 - Remover uma disciplina\n12 - Adicionar uma disciplina\n13 - Sair\n\n";
	char studMenu[] = "Menu Principal:\n 1 - Lista de Disciplinas\n 2 - Ementa de uma Disciplina\n 3 - Informacoes de uma Disciplina\n 4 - Lista de informacoes de todas as disciplinas\n 5 - Mensagens de uma disciplina\n 6 - Sair\n\n";
	
	struct sockaddr_in addr_in;
	
	sockfd = setupConnection();
	
	do {
		printf("Quem eh voce?\n1 - Professor\n2 - Aluno\n3 - Sair\n");
		printf("Opcao: ");
		scanf("%d",&persConn);
		
		int n;
		char login[6];
		n = sprintf(login, "login%d", persConn);
		
		switch(persConn){
			case 1:		
				sendMsg(sockfd, login);
				recvMsg(sockfd, buffer);
				printf("Connected: %d\n", connected);
				printf("Client received %s from server\n",buffer);
				
				if(strcmp(buffer,"connected") == 0) {
					printf("Bem-vindo professor!\n");
					printf("%s", profMenu);
					printf("Opcao: ");
					scanf("%d", &op);
					
					connected = TRUE;
				}
				else {
					printf("Nao autenticado!\n");
				}
				
				break;
			case 2:
				sendMsg(sockfd, login);
				recvMsg(sockfd, buffer);
				printf("Connected: %d\n", connected);
				printf("Client received %s from server\n",buffer);
				
				if(strcmp(buffer,"connected") == 0) {
					printf("Bem-vindo aluno!\n");
					printf("%s", studMenu);
					printf("Opcao: ");
					scanf("%d", &op);
					
					connected = TRUE;
				}
				else {
					printf("Nao autenticado!\n");
				}
				
				break;
			
			case 3:
				break;
			
			default:
				printf("Opcao inexistente!\n");
				break;
		}
		
	} while(!connected && persConn != 3);
	
	if((persConn == 1 && op == 13) || (persConn == 2 && op == 6) || persConn == 3) {
		printf("Saindo...\n");
		connected = FALSE;
		close(sockfd);
	}
	
	while(connected) {
		int n;
		char opMsg[5];
		n = sprintf(opMsg, "op%d", op);
		
		sendMsg(sockfd, opMsg);
		recvMsg(sockfd, buffer);
		
		printf("Client received %s from server\n",buffer);
		
		if(persConn == 1){
			//system("clear");
			printf("%s", profMenu);
			printf("Opcao: ");
			scanf("%d", &op);
		}
		else if(persConn == 2){
			//system("clear");
			printf("%s", studMenu);
			printf("Opcao: ");
			scanf("%d", &op);
		}
		
		if((persConn == 1 && op == 13) || (persConn == 2 && op == 6)) {
			connected = FALSE;
			printf("Saindo...\n");
			close(sockfd);
		}
	}
	
	return 0;
}