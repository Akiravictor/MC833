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
  char prof[] = "Welcome, professor!\nWhat do you want to do? Enter the command between brackets\n\n> List all disciplines [list]\n> Change message of a specific discipline [change]\n> Add discipline [add]\n> Delete discipline [delete]\n> Close connection [exit]\n\n";
 char student[] = "Welcome, student!\nWhat do you want to do? Enter the command between brackets\n\n> List all disciplines [list]\n> Close connection [exit]\n\n";

 
  /* welcome */
  obj.welcome = malloc(strlen(welcome)*sizeof(char));
  strcpy(obj.welcome ,welcome);

  /* who is this?? */
  obj.error_persona = malloc(strlen(error_persona)*sizeof(char));
  strcpy(obj.error_persona ,error_persona);

  /* Prof page */
  obj.prof = malloc(strlen(prof)*sizeof(char));
  strcpy(obj.prof ,prof);

  /* Student page */
  obj.student = malloc(strlen(student)*sizeof(char));
  strcpy(obj.student ,student);

  return obj;
}
