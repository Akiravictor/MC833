#include "includes.h"

/* comentarios gerais no server.h */


void sigchld_handler(int s){
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

messages messages_constructor(){
  messages obj;
  /* Messages */    
  char welcome[] = "|===================================|\n Welcome to VigAkira™ school system.\n Enter the command between brackets.\n Are you...\n a student [student]\n a professor [professor]\n Exit VigAkira™ school system [exit]\n";
  char error_persona[] = "Please enter 'professor' or 'student'\n";
  
  
 char student[] = "|===================================|\n Welcome, student!\n What do you want to do?\n Enter the command between brackets\n\n > List all disciplines [list]\n > Close connection [exit]\n\n";

  char prof[] = "|===================================|\n Welcome, professor!\n What do you want to do?\n Enter the command between brackets\n\n > List all disciplines [list]\n > Change message of a specific discipline [change]\n > Add discipline [add]\n > Delete discipline [delete]\n > Close connection [exit]\n\n";
  char ask_code[] = "|===================================|\n Please enter the discipline code\n e.g. 'MC833'\n";
  char ask_msg[] = "|===================================|\n Please enter the new message\n";
  char ask_room[] = "|===================================|\n Please enter the classrom info\n e.g. 'CB 15'\n";
  char ask_hours[] = "|===================================|\n Please enter the class hours\n e.g. 'ter 10am'\n";
  char ask_ementa[] = "|===================================|\n Please enter the class ementa\n e.g. 'laboratório de redes'\n";
  
  /* ask for d room */
  obj.ask_room = malloc(strlen(ask_room)*sizeof(char));
  strcpy(obj.ask_room,ask_room);

  /* ask for d ementa */
  obj.ask_ementa = malloc(strlen(ask_ementa)*sizeof(char));
  strcpy(obj.ask_ementa,ask_ementa);

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
