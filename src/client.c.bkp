#include "includes.h"


int main(){
  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  char msg[MAXDATASIZE];
  struct addrinfo hints, *serverInfo, *p;
  int status;
  double list[30], add[30], delete[30], change[30];
  clock_t t;
 
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET; //sets IPv4 use
  hints.ai_socktype = SOCK_STREAM; //TCP socket type
  hints.ai_flags = AI_PASSIVE;

  if( (status = getaddrinfo(ADDR, PORT, &hints, &serverInfo)) != 0){
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  // loop through all the results and connect to the first we can
  for(p = serverInfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
			 p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "Failed to connect\n");
    return 2;
  }
	
  freeaddrinfo(serverInfo);
	
  while(1){
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
      perror("recv");
      exit(1);
    }
		
    if(numbytes == 0){
      close(sockfd);
      exit(0);
    }

    buf[numbytes-1] = '\0';

    /* printf("%s\n",buf); */


    /* AUTOMATIZAÇÃO */
    if (send(sockfd, "professor\n", strlen("professor\n"), 0) == -1)
      perror("send");

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
      perror("recv");
      exit(1);
    }
		
    if(numbytes == 0){
      close(sockfd);
      exit(0);
    }

    buf[numbytes-1] = '\0';

    /* list */
    puts("COMMAND: [list]:");
    for(int i=0; i<30; i++){
      t = clock();
      if (send(sockfd, "list\n", strlen("lista\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      if (send(sockfd, "\n", strlen("\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      t = clock() - t;
      list[i] = ((double)t)/CLOCKS_PER_SEC;
    }


    for(int i=0; i<30; i++){
      printf("%f\n",list[i]);
    }

    /* change */
    puts("COMMAND: [change]:");
    for(int i=0; i<30; i++){
      t = clock();
      if (send(sockfd, "change\n", strlen("change\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      if (send(sockfd, "MC833\n", strlen("MC833\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      if (send(sockfd, "Essa é a nova mensagem\n", strlen("Essa é a nova mensagem\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      if (send(sockfd, "\n", strlen("\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';
      

      t = clock() - t;
      change[i] = ((double)t)/CLOCKS_PER_SEC;
    }


    for(int i=0; i<30; i++){
      printf("%f\n",change[i]);
    }



    /* add */
    puts("COMMAND: [add]:");
    for(int i=0; i<30; i++){
      t = clock();
      /* add */
      if (send(sockfd, "add\n", strlen("add\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      /* code */
      if (send(sockfd, "MC833\n", strlen("MC833\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }
      buf[numbytes-1] = '\0';

      /* sala */
      if (send(sockfd, "CB08\n", strlen("CB08\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }
      buf[numbytes-1] = '\0';

      /* horario */
      if (send(sockfd, "ter qui 17h\n", strlen("ter qui 17h\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }
      buf[numbytes-1] = '\0';

      /* msg */
      if (send(sockfd, "mensagem SHOW\n", strlen("mensagem SHOW\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }
      buf[numbytes-1] = '\0';

       /* enter */
      if (send(sockfd, "\n", strlen("\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }
      buf[numbytes-1] = '\0';

      
      

      t = clock() - t;
      add[i] = ((double)t)/CLOCKS_PER_SEC;
    }


    for(int i=0; i<30; i++){
      printf("%f\n",add[i]);
    }


     /* delete */
    puts("COMMAND: [delete]:");
    for(int i=0; i<30; i++){
      t = clock();
      if (send(sockfd, "delete\n", strlen("delete\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      if (send(sockfd, "MC833\n", strlen("MC833\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';

      if (send(sockfd, "\n", strlen("\n"), 0) == -1)
	perror("send");

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	perror("recv");
	exit(1);
      }
		
      if(numbytes == 0){
	close(sockfd);
	exit(0);
      }

      buf[numbytes-1] = '\0';
      

      t = clock() - t;
      delete[i] = ((double)t)/CLOCKS_PER_SEC;
    }


    for(int i=0; i<30; i++){
      printf("%f\n",delete[i]);
    }



    if (send(sockfd, "exit\n", strlen("exit\n"), 0) == -1)
      perror("send");

    ///////////////////////////////////////		
    /* printf(": "); */
    /* fgets(msg, MAXDATASIZE,stdin); */
		
    /* //msg[strlen(msg) - 1] = '\0'; */

    /* if (send(sockfd, msg, strlen(msg), 0) == -1) */
    /*   perror("send"); */

    /* buf[numbytes] = '\0'; */

    /* printf("client: received '%s'\n",buf);	 */
		
  }

  return 0;
}
