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
#include "../headers/client.h"
#include "../headers/server.h"
#include "../headers/disciplinas.h"

#define ADDR "localhost"
#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 2048

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
	int connected = 0;
	int menu = 0;
	int wait_recv = 0;
	
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
    int rv;
    int numbytes;
	char buf[MAXDATASIZE];
	char input[MAXDATASIZE];
	messages msg = messages_constructor();
	socklen_t addrlen;
	
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(ADDR, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
		
		/*
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
		*/
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }
	
	
	do{
		if(connected == 0){
			menu = 0;
			
			printf("%s", msg.welcome);
			fgets(buf, MAXDATASIZE, stdin);
			buf[strlen(buf) - 1] = '\0';
			
			printf("DEBUG: eu li: %s\n", buf);
			
			if(strcmp(buf,"student") == 0){
				connected = 1;
				sprintf(buf,"STD");
				printf("DEBUG: aluno validado\n");
			}
			else if(strcmp(buf,"professor") == 0){
				connected = 2;
				sprintf(buf,"PRO");
				printf("DEBUG: professor validado\n");
			}
			else if(strcmp(buf,"exit") == 0){
				connected = 0;
			}
			else{
				printf("Invalid option!\n");
			}
			
			if(connected != 0){
				sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
			}
		}
		else if(connected != 0){
			
			if(menu == 0){
				addrlen = sizeof their_addr;
				
				if( (numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1, 0, (struct sockaddr*) &their_addr, &addrlen)) == -1){
					perror("recvfrom");
					exit(1);
				}
				
				buf[numbytes] = '\0';
				printf("DEBUG: received: %s\n\n", buf);
			}
			
			if(strcmp(buf, "MENU") == 0){
				if(connected == 1){
					printf("%s", msg.student);
				}
				else if(connected == 2){
					printf("%s", msg.prof);
				}
				menu = 1;
			}
			
			if(menu == 1){
				
				//read menu
				fgets(buf, MAXDATASIZE, stdin);
				buf[strlen(buf) - 1] = '\0';
				
				if(strcmp(buf, "list") == 0){
					sprintf(buf, "LST");
					
					wait_recv = 1;
					
				}
				else if(strcmp(buf, "change") == 0 && connected == 2){
					char code[10];
					char message[255];
					
					printf("%s", msg.ask_code);
					//Read code
					fgets(code, 10, stdin);
					code[strlen(code) - 1] = '\0';
					
					printf("%s", msg.ask_msg);
					//Read message
					fgets(message, 255, stdin);
					message[strlen(message) - 1] = '\0';
					
					sprintf(buf, "CHG|COD%s|MSG%s",code, message);
					
					wait_recv = 1;
					
				}
				else if(strcmp(buf, "add") == 0 && connected == 2){
					char code[10];
					char sala[10];
					char horarios[50];
					char mensagem[255];
					char ementa[255];
					
					printf("%s", msg.ask_code);
					//Read code
					fgets(code, 10, stdin);
					code[strlen(code) - 1] = '\0';
					
					printf("%s", msg.ask_room);
					//Read code
					fgets(sala, 10, stdin);
					sala[strlen(sala) - 1] = '\0';
					
					printf("%s", msg.ask_hours);
					//Read code
					fgets(horarios, 50, stdin);
					horarios[strlen(horarios) - 1] = '\0';
					
					printf("%s", msg.ask_msg);
					//Read message
					fgets(mensagem, 255, stdin);
					mensagem[strlen(mensagem) - 1] = '\0';
					
					printf("%s", msg.ask_ementa);
					//Read message
					fgets(ementa, 255, stdin);
					ementa[strlen(ementa) - 1] = '\0';
					
					sprintf(buf, "ADD|COD%s|ROM%s|HRS%s|MSG%s|EMT%s", code, sala, horarios, mensagem, ementa);
					
					wait_recv = 1;
					
				}
				else if(strcmp(buf, "delete") == 0 && connected == 2){
					char code[10];
					
					printf("%s", msg.ask_code);
					//Read code
					fgets(code, 10, stdin);
					code[strlen(code) - 1] = '\0';
					
					sprintf(buf, "DEL|COD%s",code);
					
					wait_recv = 1;
					
				}
				else if(strcmp(buf, "exit") == 0){
					connected = 0;
				}
				else {
					printf("Invalid option!\n");
				}
				
				if(connected != 0){
					sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
				}
				
				if(connected != 0 && wait_recv == 1){
					
					printf("DEBUG: waiting recvfrom...\n");
					
					if( (numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1, 0, (struct sockaddr*) &their_addr, &addrlen)) == -1){
						perror("recvfrom");
						exit(1);
					}
					
					buf[numbytes] = '\0';
					printf("DEBUG: received: %s\n\n", buf);
					
					fgets(input, MAXDATASIZE, stdin);
					input[strlen(input) - 1] = '\0';
					
					wait_recv = 0;
					
				}

				sprintf(buf, "MENU");
			}
		}
		
	}while(connected == 1 || connected == 2);
	
    freeaddrinfo(servinfo);
	
    close(sockfd);

    return 0;
}