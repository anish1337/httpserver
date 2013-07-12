/*
 *
 *
 *		Authors:	Anish Pednekar
 *
 *
 *
 */


#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define PORT 5010			// port number to listen on
#define BUFFLEN 50			// max length to read

int main()
{
  // sockets
  int sock_listen;
  int sock_connect;

  // structure to store address details
  struct sockaddr_in servAddr;

  char buff[BUFFLEN];
  ssize_t rcount;


  // files
  FILE* sfile;
  char fname[BUFFLEN];

  // clear the servAddr structure and initialize
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(PORT);

  // initialize the listen socket
  sock_listen = socket(AF_INET, SOCK_STREAM, 0);

  // bind sock_listen to PORT
  if(bind(sock_listen, (struct sockaddr*) &servAddr, sizeof(servAddr))==-1)
    {
      printf("Error binding\n");
      exit(EXIT_FAILURE);
    }

  while(1)
    {
      // listen for incomming connections
      if(listen(sock_listen, 2)==-1)
	{
	  printf("Error listening...\n");
	  exit(EXIT_FAILURE);
	}

      // accept the connection
      if((sock_connect = accept(sock_listen, NULL, NULL))==-1)
	{
	  printf("Error accepting connection\n");
	  exit(EXIT_FAILURE);
	}

		
      memset(buff, '\0', BUFFLEN);
      memset(fname, '\0', BUFFLEN);
      
      // read data from the connection and store
      // it in buff
      if((rcount = read(sock_connect, buff, BUFFLEN))==-1)
	{
	  printf("Error reading\n");
	  exit(EXIT_FAILURE);
	}

      // exit if client sends 'q'
      if(buff[0]=='q')
	break;

      // print buff to stdout
      for(ssize_t i=0; i<rcount; i++)
	{
	  printf("%c", buff[i]);
	  fname[i] = buff[i];
	}
      
      // to exclude the ^M newline char
      fname[rcount-2] = '\0';

      // open the file for reading if it exists
      // and write the contents to the socket
      if((sfile = fopen(fname, "r"))==NULL)
	write(sock_connect, "File not found!\n", 16);

      else
	{
	  char ch;
	  while((ch = fgetc(sfile))!= EOF)
	    write(sock_connect, &ch, sizeof(ch));
	}

      // close the connection
      if(close(sock_connect)==-1)
	{
	  printf("Error closing socket");
	  exit(EXIT_FAILURE);
	}


      close(sock_connect);
      
      if(sfile!=NULL)
	fclose(sfile);
    }
  
  close(sock_listen);
  exit(EXIT_SUCCESS);
}
