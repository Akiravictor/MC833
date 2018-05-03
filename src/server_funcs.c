#include "includes.h"

void sigchld_handler(int s){
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

int setupConnectionS(int max_clients, int* client_socket) {
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

void sendMsg(int sock, char* msg) {
	if( send(sock, msg, strlen(msg), 0) == -1) {
		perror("Server: send");
		exit(1);
	}
}

int recvMsgS(int sock, char* buffer) {
	int buf_size;
	
	buffer[0] = '\0';
	
	buf_size = recv(sock, buffer, MAXDATASIZE -1, 0);
	
	if( buf_size == -1) {
		perror("Server: receive");
		exit(1);
	}
	else if( buf_size == 0) {
		//close(sock);
	}
	else {
		//int size = buf_size -1;
		buffer[buf_size] = '\0';
	}
	return buf_size;
}

void executeMenu(int whoIsConnected, char* buffer, lista *l, int sockfd) {
	
	if(strcmp(buffer,"op1") == 0) {
		
	}
	else if(strcmp(buffer,"op2") == 0) {
		
	}
	else if(strcmp(buffer,"op3") == 0) {
		
	}
	else if(strcmp(buffer,"op4") == 0) {
		
	}
	else if(strcmp(buffer,"op5") == 0) {
		
	}
	else if(strcmp(buffer,"op6") == 0) {
		int size;
		int end = 0;
		char codigo[10];
		char sala[10];
		char horarios[50];
		char ementa[255];
		char titulo[100];
		
		do {
			printf("execMenu\n");
			printf("fd: %d\n", sockfd);
			sendMsg(sockfd, "codigo da disciplina: ");
			printf("Message sent!\n");
			size = recvMsgS(sockfd, buffer);
			printf("Recebi %s\n", buffer);
			
			if(size > 10) {
				sprintf(buffer, "codigo da disciplina com tamanho invalido! (max 10)\n");
				sendMsg(sockfd, buffer);
			}
			else {
				//Disciplina válida
				sprintf(codigo,"%s",buffer);
				
				sprintf(buffer, "sala: ");
				sendMsg(sockfd, buffer);
				size = recvMsgS(sockfd, buffer);
				
				if(size > 10) {
					sprintf(buffer, "sala com tamanho invalido! (max 10)\n");
					sendMsg(sockfd, buffer);
				}
				else {
					//Sala válida
					sprintf(sala,"%s",buffer);
				
					sprintf(buffer, "horarios: ");
					sendMsg(sockfd, buffer);
					size = recvMsgS(sockfd, buffer);
					
					if(size > 50) {
						sprintf(buffer, "horario com tamanho invalido! (max 50)\n");
						sendMsg(sockfd, buffer);
					}
					else {
						//Horario válido
						sprintf(horarios,"%s",buffer);
				
						sprintf(buffer, "ementa: ");
						sendMsg(sockfd, buffer);
						size = recvMsgS(sockfd, buffer);
						
						if(size > 255) {
							sprintf(buffer, "ementa com tamanho invalido! (max 255)\n");
							sendMsg(sockfd, buffer);
						}
						else {
							//Ementa valida
							sprintf(ementa,"%s",buffer);
					
							sprintf(buffer, "titulo: ");
							sendMsg(sockfd, buffer);
							size = recvMsgS(sockfd, buffer);
							if(size > 100) {
								sprintf(buffer, "titulo com tamanho invalido! (max 100)\n");
								sendMsg(sockfd, buffer);
							}
							else {
								//Titulo valido
								sprintf(titulo,"%s",buffer);
								sprintf(buffer, "end");
								sendMsg(sockfd, buffer);
								end = 1;
							}
						}
					}
				}
			}
			
		} while(!end);
		
	}
	else if(strcmp(buffer,"op7") == 0) {
		
	}
	else if(strcmp(buffer,"op8") == 0) {
		
	}
	else if(strcmp(buffer,"op9") == 0) {
		
	}
	else if(strcmp(buffer,"op10") == 0) {
		
	}
	else if(strcmp(buffer,"op11") == 0) {
		
	}
	else if(strcmp(buffer,"op12") == 0) {
		
	}
}

messages messages_constructor(){
  messages obj;
  /* Messages */
  char welcome[] = "Welcome to VigAkira™ school system.\nAre you a student or professor?\n'exit' closes connection\n";
  char error_persona[] = "Please enter 'professor' or 'student'\n";
  
  
 char student[] = "Welcome, student!\nWhat do you want to do? Enter the command between brackets\n\n> List all disciplines [list]\n> Close connection [exit]\n\n";

  char prof[] = "Welcome, professor!\nWhat do you want to do? Enter the command between brackets\n\n> List all disciplines [list]\n> Change message of a specific discipline [change]\n> Add discipline [add]\n> Delete discipline [delete]\n> Close connection [exit]\n\n";
  char ask_code[] = "Please enter the discipline code, e.g. 'MC833'\n";
  char ask_msg[] = "Please enter the new message\n";
  char ask_room[] = "Please enter the classrom info, e.g. 'CB 15'\n";
  char ask_hours[] = "Please enter the class hours, e.g. 'ter 10am'\n";
  
  /* ask for d room */
  obj.ask_room = malloc(strlen(ask_room)*sizeof(char));
  strcpy(obj.ask_room,ask_room);

  /* ask for d hours */
  obj.ask_hours = malloc(strlen(ask_hours)*sizeof(char));
  strcpy(obj.ask_hours,ask_hours);

  /* ask for d code */
  obj.ask_code = malloc(strlen(ask_code)*sizeof(char));
  strcpy(obj.ask_code,ask_code);

  /* ask for d msg */
  obj.ask_msg = malloc(strlen(ask_msg)*sizeof(char));
  strcpy(obj.ask_msg,ask_msg);
  
  /* welcome */
  obj.welcome = malloc(strlen(welcome)*sizeof(char));
  strcpy(obj.welcome ,welcome);

  /* who is this?? */
  obj.error_persona = malloc(strlen(error_persona)*sizeof(char));
  strcpy(obj.error_persona ,error_persona);

  /* Student page */
  obj.student = malloc(strlen(student)*sizeof(char));
  strcpy(obj.student ,student);

    /* Prof page */
  obj.prof = malloc(strlen(prof)*sizeof(char));
  strcpy(obj.prof ,prof);


  return obj;
}

void send_and_receive(int incoming_fd, char *string, int *buf_size, char *buffer){

  if(send(incoming_fd, string ,strlen(string) , 0) == -1){
    perror("server: send");
  }
  else if( (*buf_size = recv(incoming_fd, buffer, MAXDATASIZE-1, 0)) == -1){
    perror("server: receive");
    exit(1);
  }
  if(*buf_size == 0){
   close(incoming_fd);
   exit(0);
  }
}
