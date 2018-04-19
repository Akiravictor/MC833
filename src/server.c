#include "includes.h"

#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 255

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
  char *list, *delete, *code, *d_msg, *room, *hours;
  int connected;
  pid_t process;
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
  while(1){
    printf("server: waiting for connections...\n");

    sin_size = sizeof client_addr;


    //Accepts connection
    if( (incoming_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size)) == -1){
      perror("server: accept");
    }
    connected = 1;
    process = fork();
    /* erro */
    if(process < 0){
      perror("Ih rapaz...\n");
    }
    /* Parent */
    if(process == 0){
      printf("SERVER_PROCESS: I am parent %d of %d\n",getpid(),process);
      wait(NULL);
    }
    else{
      printf("SERVER_PROCESS: I am child %d of %d\n",getpid(),getppid());

      //Converts IPv4 and IPv6 addresses from binary to text form
      inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*) &client_addr),s, sizeof s);
      printf("server: connected to %s\n", s);

      /* Initial Message */
      send_and_receive(incoming_fd,msg.welcome,&buf_size,buffer);
      while(connected){

	if(buf_size > 0){
	  buffer[buf_size] = '\0';
	  printf("server: received %s\n", buffer);

	  /* Exits */
	  if(strcmp(buffer,"exit\r\n") == 0){
	    if(send(incoming_fd, "Closing Connection fella!", 25, 0) == -1){
	      perror("server: send");
	    }
	    close(incoming_fd);
	    connected=0;
	  }
      
	  /* Student */
	  else if(strcmp(buffer,"student\r\n") == 0){
	    puts("server: student selected");
	    while(connected){
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
		  connected = 0;
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
	    puts("server: professor selected");
	    while(connected){
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
		  connected = 0;
		}
		/* [list] */
		else if(strcmp(buffer,"list\r\n") == 0){
		  list = p_list(l);
		  send_and_receive(incoming_fd,list,&buf_size,buffer);
		}
		/* [change] */
		else if(strcmp(buffer,"change\r\n") == 0){
		  send_and_receive(incoming_fd,msg.ask_code,&buf_size,buffer);
		  if(buf_size > 0){
		    buffer[buf_size-2] = '\0';
		    printf("server: received %s\n", buffer);
		    code = (char*)malloc((buf_size-2)*sizeof(char));
		    strcpy(code,buffer);
		    send_and_receive(incoming_fd,msg.ask_msg,&buf_size,buffer);
		    if(buf_size > 0){
		      buffer[buf_size-2] = '\0';
		      printf("server: received %s\n", buffer);
		      d_msg = c_message(&l,code,buffer);
		      send_and_receive(incoming_fd,d_msg,&buf_size,buffer);
		    }
		  }
		}
		/* [add] */
		else if(strcmp(buffer,"add\r\n") == 0){
		  /* ask for code */
		  send_and_receive(incoming_fd,msg.ask_code,&buf_size,buffer);
		  if(buf_size > 0){
		    buffer[buf_size-2] = '\0';
		    printf("server: received %s\n", buffer);
		    code = (char*)malloc((buf_size-2)*sizeof(char));
		    strcpy(code,buffer);
		    /* ask for room id */
		    send_and_receive(incoming_fd,msg.ask_room,&buf_size,buffer);
		    if(buf_size > 0){
		      buffer[buf_size-2] = '\0';
		      printf("server: received %s\n", buffer);
		      room = (char*)malloc((buf_size-2)*sizeof(char));
		      strcpy(room,buffer);
		      /* ask for class hours */
		      send_and_receive(incoming_fd,msg.ask_hours,&buf_size,buffer);
		      if(buf_size > 0){
			buffer[buf_size-2] = '\0';
			printf("server: received %s\n", buffer);
			hours = (char*)malloc((buf_size-2)*sizeof(char));
			strcpy(hours,buffer);
			/* ask for msg */
			send_and_receive(incoming_fd,msg.ask_msg,&buf_size,buffer);
			if(buf_size > 0){
			  buffer[buf_size-2] = '\0';
			  printf("server: received %s\n", buffer);
			  d_msg = a_disciplina(&l,code,room,hours,buffer);
			  send_and_receive(incoming_fd,d_msg,&buf_size,buffer);
			}
		      }
		    }
		  }
		}
		/* [delete] */
		else if(strcmp(buffer,"delete\r\n") == 0){
		  send_and_receive(incoming_fd,msg.ask_code,&buf_size,buffer);
		  if(buf_size > 0){
		    buffer[buf_size-2] = '\0';
		    printf("server: received %s\n", buffer);
		    delete = d_disc(&l,buffer);
		    send_and_receive(incoming_fd,delete,&buf_size,buffer);
		  }
		}
	      }
	    }
	  }
	}
	/* Who are you?!?!?!?! */
	if(connected)
	  send_and_receive(incoming_fd,msg.error_persona,&buf_size,buffer);
      }
    }
  }
  puts("DEBUG: out");
  return 0;
}

