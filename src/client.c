#include "includes.h"



int main(){

  int a=10,b=1;
  
  while(1){
    /* filho */
    if(fork()==0){
      a++;
      b++;
      printf("filho %d de %d: %d %d\n",getpid(),getppid(),a,b);
    }
    else{
      printf("pai %d: %d %d\n",getpid(),a,b);
      wait(NULL);
    }
    sleep(1);
  }
}
