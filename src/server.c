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


#define MAXBUFLEN 100


int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXDATASIZE];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
	
	lista l = lista_constructor();
	add_disciplina(&l,"MC833","IC 352","qui 10h am","o prof edmundo é show","Lab de redes fazemos labs legais");
	add_disciplina(&l,"MC558","CB 17","ter qui 4h pm","tá O(n) pelo menos","Grafos, grafos, grafos");
	add_disciplina(&l,"MC722","CB 5","ter qui 7h pm","MIIIIIPPPPPSSSS","Arquitetura de computadores");

	messages msg = messages_constructor();
	char *list, *delete, *code, *d_msg, *room, *hours, *ementa;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

	while(1){
		char op[4];
		int i;
		
		memset(op, '\0', sizeof op);
		
		printf("listener: waiting to recvfrom...\n");

		addr_len = sizeof their_addr;
		if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		
		printf("DEBUG: got packet from %s\n",
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
		printf("DEBUG: packet is %d bytes long\n", numbytes);
		buf[numbytes] = '\0';
		printf("DEBUG: packet contains \"%s\"\n", buf);
		
		//Decode message
		for(i = 0; i < 3; i++){
			op[i] = buf[i];
		}
		op[4] = '\0';
		
		if(strcmp(op, "STD") == 0){
			printf("DEBUG: student_confirmed\n");
			sendto(sockfd, "MENU", 4, 0, (struct sockaddr *)&their_addr, addr_len);
		}
		else if(strcmp(op, "PRO") == 0){
			printf("DEBUG: professor_confirmed\n");
			sendto(sockfd, "MENU", 4, 0, (struct sockaddr *)&their_addr, addr_len);
		}
		else if(strcmp(op, "LST") == 0){
			printf("DEBUG: LST confirmed\n");
			list = p_list(l);
			sprintf(buf, "%s", list);
			sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&their_addr, addr_len);
		}
		else if(strcmp(op, "CHG") == 0){
			char code[10];
			char message[255];
			char op[4];
			int i;
			int j = 0;
			int read_op = 1;
			int op_strt = 4;
			int op_done = 0;
			
			memset(code, '\0', sizeof code);
			memset(message, '\0', sizeof message);
			memset(op, '\0', sizeof op);
			
			printf("DEBUG: CHG confirmed\n");
			
			for(i = 4; i < strlen(buf); i++){
				if(read_op == 1){
					op[j] = buf[i];
					j++;
					
					if(i == (op_strt + 2)){
					
						op[op_strt] = '\0';
						read_op = 0;
						op_done = 1;
						j = 0;
						i++;
					}
				}
				
				if(op_done == 1){
					
					if(strcmp(op, "COD") == 0){
						code[j] = buf[i];
					}
					else if(strcmp(op, "MSG") == 0){
						message[j] = buf[i];
					}
					
					if(buf[i] == '|'){
						code[j] = '\0';

					}
					else if(buf[i] == '\0'){
						message[j] = '\0';
					}

					j++;
					
					if(buf[i] == '|' || buf[i] == '\0'){
						read_op = 1;
						op_strt = i + 1;
						op_done = 0;
						j = 0;
					}
					
				}
				
			} //End-for
			
			printf("DEBUG:\n\tCode: %s\n\tMessage: %s\n", code, message);
			
			d_msg = c_message(&l, code, message);
			sprintf(buf, "%s", d_msg);
			
			sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&their_addr, addr_len);
			
		}
		else if(strcmp(op, "ADD") == 0){
			char code[10];
			char sala[10];
			char horarios[50];
			char message[255];
			char ementa[255];
			char op[4];
			int i;
			int j = 0;
			int pipe_count = 0;
			int read_op = 1;
			int op_strt = 4;
			int op_done = 0;
			
			memset(code, '\0', sizeof code);
			memset(sala, '\0', sizeof sala);
			memset(horarios, '\0', sizeof horarios);
			memset(message, '\0', sizeof message);
			memset(ementa, '\0', sizeof ementa);
			memset(op, '\0', sizeof op);
			
			printf("DEBUG: ADD confirmed\n");
			
			for(i = 4; i < strlen(buf); i++){
				if(read_op == 1){
					op[j] = buf[i];
					j++;
					
					if(i == (op_strt + 2)){
					
						op[op_strt] = '\0';
						read_op = 0;
						op_done = 1;
						j = 0;
						i++;
					}
				}
				
				if(op_done == 1){
					
					if(strcmp(op, "COD") == 0){
						code[j] = buf[i];
					}
					else if(strcmp(op, "ROM") == 0){
						sala[j] = buf[i];
					}
					else if(strcmp(op, "HRS") == 0){
						horarios[j] = buf[i];
					}
					else if(strcmp(op, "MSG") == 0){
						message[j] = buf[i];
					}
					else if(strcmp(op, "EMT") == 0){
						ementa[j] = buf[i];
					}
					
					if(buf[i] == '|'){
						if(pipe_count == 0){
							code[j] = '\0';
							pipe_count++;
						}
						else if(pipe_count == 1){
							sala[j] = '\0';
							pipe_count++;
						}
						else if(pipe_count == 2){
							horarios[j] = '\0';
							pipe_count++;
						}
						else if(pipe_count == 3){
							message[j] = '\0';
							pipe_count++;
						}
						
					}
					else if(buf[i] == '\0'){
						ementa[j] = '\0';
					}

					j++;
					
					if(buf[i] == '|' || buf[i] == '\0'){
						read_op = 1;
						op_strt = i + 1;
						op_done = 0;
						j = 0;
					}
					
				}
				
			} //End-for
			
			
			printf("DEBUG:\n\tCode: %s\n\tSala: %s\n\tHorarios: %s\n\tMessage: %s\n\tEmenta: %s\n", code, sala, horarios, message, ementa);
			
			d_msg = a_disciplina(&l, code, sala, horarios, message, ementa);
			sprintf(buf, "%s", d_msg);
			
			sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&their_addr, addr_len);
			
		}
		else if(strcmp(op, "DEL") == 0){
			char code[10];
			char op[4];
			int i;
			int j = 0;
			int read_op = 1;
			int op_strt = 4;
			int op_done = 0;
			
			memset(code, '\0', sizeof code);
			memset(op, '\0', sizeof op);
			
			printf("DEBUG: DEL confirmed\n");
			
			for(i = 4; i < strlen(buf); i++){
				printf("buf[%d]: %c\n", i, buf[i]);
				
				if(read_op == 1){
					op[j] = buf[i];
					j++;
					
					if(i == (op_strt + 2)){
					
						op[op_strt + 1] = '\0';
						read_op = 0;
						op_done = 1;
						j = 0;
						i++;
					}
				}
				
				if(op_done == 1){
					if(strcmp(op, "COD") == 0){
						code[j] = buf[i];
					}
					
					else if(buf[i] == '\0'){
						code[j] = '\0';
					}

					j++;
					
				}
				
			} //End-for
			
			printf("DEBUG:\n\tCode: %s\n", code);
			
			d_msg = d_disc(&l, code);
			sprintf(buf, "%s", d_msg);
			
			sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&their_addr, addr_len);
			
		}
		else{
			printf("DEBUG: op not recognized\n");
		}
		
	}
    

    close(sockfd);

    return 0;
}