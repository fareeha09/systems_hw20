#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
	//creates upstream pipe and is waiting for client to respond
	int pip = mkfifo("upstream", 0644);
	printf("[Server] made the upstream pipe... now waiting for client...\n"); 
	
	//client connects to the well known pipe (wkp) 
	//and then server removes it 
	int up = open("upstream", O_RDONLY);
	printf("[Server] client connected.. removed the well known pipe\n");
	remove("upstream");
	
	//server now reads the name of the pipe from the client
	char readin[HANDSHAKE_BUFFER_SIZE];
	read(up, readin, HANDSHAKE_BUFFER_SIZE);
	printf("[Server] name of private pipe received from client\n");
	
	//server opens the private client's pipe
	int down = open(readin, O_WRONLY);
	write(down, ACK, sizeof(ACK));
	printf("[Server] sending message 'HOLA' to client\n");
	
	printf("[Server] done. all connected.\n");
	
	*to_client = down;
	return up;
}

/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
	
	//connects to the server
	int up = open("upstream", O_WRONLY);
	printf("[Client] connected to server\n");
	
	//creates a downstream pipe then sends the name of that to the server
	char message[HANDSHAKE_BUFFER_SIZE];
	strcpy(message, "down");
	int down = mkfifo(message, 0644);
	printf("[Client] created privated pipe (downstream fifo)\n");
	write(up, message, HANDSHAKE_BUFFER_SIZE);
	printf("[Client] sent name of private pipe to server \n");
	
	//client receives message from server
	//then sends back message that he received it
	printf("[Client] message 'HOLA' received.. sending 'HOLA' back \n");
	write(up, ACK, HANDSHAKE_BUFFER_SIZE);
	
	*to_server = up;
	return down;
}