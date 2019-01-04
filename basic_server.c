#include "pipe_networking.h"
#include <signal.h>

static void sighandler(int signo){
	if (signo == SIGINT){
		remove("pipe");
		exit(EXIT_SUCCESS);
	}
}

int main() {
  signal(SIGINT,sighandler);
  
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  
  while(1){
  //gets data from client
  char received[100];
  int reset = read(from_client, received, 100);
  
  //process data
  char new[100];
  strncpy(new, received, strlen(received)-1);
    
  //send to client
  write(to_client, new, 100);
	}
}