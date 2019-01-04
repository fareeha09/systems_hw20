#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  
  printf("What do you want to send to the server?\n");
  char input[100];
  fgets(input, 100, stdin);

  while(1){  
  //sends message to server
  write(to_server, input, 100);
  
  //receives message from server
  char received[100];
  read(from_server, received, 100);
  
  //displays message to user
  printf("[Server] %s\n", received);
}
}