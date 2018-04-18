#include "includes.h"

int main(){
  int status;
  int sockfd;
  int incoming_fd;
  int yes=1;
  struct sigaction sa;
  struct addrinfo hints;
  struct addrinfo *serverInfo;
  struct addrinfo *p;
  socklen_t sin_size;
  struct sockaddr_storage client_addr;
  char s[INET6_ADDRSTRLEN];
  char buffer[MAXDATASIZE];
  int buf_size=0;
  messages msg = messages_constructor();
  char *list;
  /* LISTA DE DISCIPLINAS */
  lista l = lista_constructor();
  add_disciplina(&l,"MC833","IC 352","qui 10h am","o prof edmundo é show");
  add_disciplina(&l,"MC558","CB 17","ter qui 4h pm","tá O(n) pelo menos");
  add_disciplina(&l,"MC722","CB 5","ter qui 7h pm","MIIIIIPPPPPSSSS");

  /////////////////////////////////////////////////////////////////////
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET; //sets IPv4 use
  hints.ai_socktype = SOCK_STREAM; //TCP socket type
  hints.ai_flags = AI_PASSIVE;

  //Setting up connection
  if( (status = getaddrinfo(NULL, PORT, &hints, &serverInfo)) != 0){
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  //Looping through the results and binding to the 1st possible
  for(p = serverInfo; p != NULL; p = p->ai_next){

    //Setting socket
    if( (sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1){
      perror("server: socket");
      continue;
    }

    //Letting the address/port being reusable
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
      perror("setsockopt");
      exit(1);
    }

    //Binding process to port
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(serverInfo);

  if(p == NULL){
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  //Listen and wait for connections
  if(listen(sockfd, QUEUE) == -1){
    perror("server: listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  sin_size = sizeof client_addr;

  //Accepts connection
  if( (incoming_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size)) == -1){
    perror("server: accept");
  }

  //Converts IPv4 and IPv6 addresses from binary to text form
  inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*) &client_addr),s, sizeof s);
  printf("server: connected to %s\n", s);

  /* Initial message */
  /* if(send(incoming_fd, msg.welcome ,strlen(msg.welcome) , 0) == -1){ */
  /*   perror("server: send"); */
  /* } */
  /* else if( (buf_size = recv(incoming_fd, buffer, MAXDATASIZE-1, 0)) == -1){ */
  /*   perror("server: receive"); */
  /*   exit(1); */
  /* } */

  /* Initial Message */
  send_and_receive(incoming_fd,msg.welcome,&buf_size,buffer);
  while(1){

    if(buf_size > 0){
      buffer[buf_size] = '\0';
      printf("server: received %s\n", buffer);

      /* Exits */
      if(strcmp(buffer,"exit\r\n") == 0){
	if(send(incoming_fd, "Closing Connection fella!", 25, 0) == -1){
	  perror("server: send");
	}
	close(incoming_fd);
	exit(0);
      }
      
      /* Student */
      else if(strcmp(buffer,"student\r\n") == 0){
	puts("aluno selecionado");
	while(1){
	  /* Student menu */
	  send_and_receive(incoming_fd,msg.student,&buf_size,buffer);
	  if(buf_size > 0){
	    buffer[buf_size] = '\0';
	    printf("server: received %s\n", buffer);

	    /* Exits */
	    if(strcmp(buffer,"exit\r\n") == 0){
	      if(send(incoming_fd, "Closing Connection fella!", 25, 0) == -1){
		perror("server: send");
	      }
	      close(incoming_fd);
	      exit(0);
	    }
	    /* [list] */
	    else if(strcmp(buffer,"list\r\n") == 0){
	      list = p_list(l);
	      send_and_receive(incoming_fd,list,&buf_size,buffer);
	    }
	    
	  }
	}
      }

      /* Professor */
      else if(strcmp(buffer,"professor\r\n") == 0){
	puts("professor selecionado");
	while(1){
	  /* Prof menu */
	  send_and_receive(incoming_fd,msg.prof,&buf_size,buffer);
	  if(buf_size > 0){
	    buffer[buf_size] = '\0';
	    printf("server: received %s\n", buffer);

	    /* Exits */
	    if(strcmp(buffer,"exit\r\n") == 0){
	      if(send(incoming_fd, "Closing Connection fella!", 25, 0) == -1){
		perror("server: send");
	      }
	      close(incoming_fd);
	      exit(0);
	    }
	    /* [list] */
	    else if(strcmp(buffer,"list\r\n") == 0){
	      list = p_list(l);
	      send_and_receive(incoming_fd,list,&buf_size,buffer);
	    }
	  }
	}
      }



    }
    /* Who are you?!?!?!?! */
     if(send(incoming_fd, msg.error_persona ,strlen(msg.error_persona) , 0) == -1){
       perror("server: send");
     }
     else if( (buf_size = recv(incoming_fd, buffer, MAXDATASIZE-1, 0)) == -1){
       perror("server: receive");
       exit(1);
     }
  }

  return 0;
}

