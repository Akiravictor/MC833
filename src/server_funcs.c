#include "includes.h"

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
  char welcome[] = "Welcome to VigAkira™ school system.\nAre you a student or professor?\n'exit' closes connection\n";
  char error_persona[] = "Please enter 'professor' or 'student'\n";
  
  
 char student[] = "Welcome, student!\nWhat do you want to do? Enter the command between brackets\n\n> List all disciplines [list]\n> Close connection [exit]\n\n";

  char prof[] = "Welcome, professor!\nWhat do you want to do? Enter the command between brackets\n\n> List all disciplines [list]\n> Change message of a specific discipline [change]\n> Add discipline [add]\n> Delete discipline [delete]\n> Close connection [exit]\n\n";
  char ask_code[] = "Please enter the discipline code, e.g. 'MC833'\n";

  /* ask for d code */
  obj.ask_code = malloc(strlen(ask_code)*sizeof(char));
  strcpy(obj.ask_code,ask_code);
  
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
