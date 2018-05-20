#include "includes.h"

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
  messages msg = messages_constructor();
  socklen_t addrlen;

  /* commands */
  char code[10];
  char message[255];
  char sala[10];
  char horarios[50];
  char ementa[255];

  long double timer;
  clock_t t;
	
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

      /* Auto select professor */
      strcpy(buf,"professor");
			
			
      if(strcmp(buf,"professor") == 0){
	connected = 2;
	sprintf(buf,"PRO");
	printf("DEBUG: professor validado\n");
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

	/* LIST */
	puts("COMMAND LIST");
	for(int i=0; i<30; i++){

	  t = clock();
	  
	  sprintf(buf, "LST");
	  wait_recv = 1;
					
	  if(connected != 0){
	    sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
	  }
				
	  if(connected != 0 && wait_recv == 1){
					
	    if( (numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1, 0, (struct sockaddr*) &their_addr, &addrlen)) == -1){
	      perror("recvfrom");
	      exit(1);
	    }
					
	  }

	  timer = ((long double)(clock() - t)/CLOCKS_PER_SEC);
	  printf("%Lf\n",timer);
	}

	puts("COMMAND CHANGE");
	for(int i=0; i<30; i++){
	  	  t = clock();
		  
	  strcpy(code,"MC833");
	  sprintf(message,"Message: %2d",i);
	  sprintf(buf, "CHG|COD%s|MSG%s",code, message);
	  wait_recv = 1;

	  if(connected != 0){
	    sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
	  }
				
	  if(connected != 0 && wait_recv == 1){
					
	    if( (numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1, 0, (struct sockaddr*) &their_addr, &addrlen)) == -1){
	      perror("recvfrom");
	      exit(1);
	    }
					
	  }
	  timer = ((long double)(clock() - t)/CLOCKS_PER_SEC);
	  printf("%Lf\n",timer);
	}

	puts("COMMAND ADD");
	for(int i=0; i<30; i++){
	  	  t = clock();
	  sprintf(code,"mc%03d",i);
	  sprintf(sala,"CB %2d",i);
	  sprintf(horarios,"10:%2d",i);
	  sprintf(message,"Message: %2d",i);
	  sprintf(ementa,"Message: %2d",i);

	  sprintf(buf, "ADD|COD%s|ROM%s|HRS%s|MSG%s|EMT%s", code, sala, horarios, message, ementa);
	  wait_recv = 1;

	  if(connected != 0){
	    sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
	  }
				
	  if(connected != 0 && wait_recv == 1){
					
	    if( (numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1, 0, (struct sockaddr*) &their_addr, &addrlen)) == -1){
	      perror("recvfrom");
	      exit(1);
	    }
					
	  }
	  timer = ((long double)(clock() - t)/CLOCKS_PER_SEC);
	  printf("%Lf\n",timer);
	}

	puts("COMMAND DELETE");
	for(int i=0; i<30; i++){
	  	  t = clock();
		  
	  sprintf(code,"mc%03d",i);
	  sprintf(buf, "DEL|COD%s",code);
	  wait_recv = 1;

	  if(connected != 0){
	    sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
	  }
				
	  if(connected != 0 && wait_recv == 1){
					
	    if( (numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1, 0, (struct sockaddr*) &their_addr, &addrlen)) == -1){
	      perror("recvfrom");
	      exit(1);
	    }
					
	  }
	  timer = ((long double)(clock() - t)/CLOCKS_PER_SEC);
	  printf("%Lf\n",timer);
	}
	
	/* disconnect */
	connected = 0;
      }
    }
		
  }while(connected == 1 || connected == 2);
	
  freeaddrinfo(servinfo);
	
  close(sockfd);

  return 0;
}
