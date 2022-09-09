//***************************************************************************
// File name:		httpget.c
// Author:			Ashley Ung
// Date:				September 22, 2021
// Purpose:			Practice writing a TCP Client
//***************************************************************************

#define _GNU_SOURCE

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/****************************************************************************
 Function:		main
 
 Description:	make a single HTTP request. Display the request and response
 
 Parameters:	int argc     - number of command line arguments
							char **argv  - the command line arguments
 
 Returned:		EXIT_SUCCESS
****************************************************************************/
int main (int argc, char **argv)
{
  const int MAX_SIZE = 4096; 
  const int HTTP_PORT = 80;
  
  int socketfd; 
  int result;
  struct sockaddr_in sAddr;
  char szGetReq[MAX_SIZE];
  char szResponse[MAX_SIZE];

  szGetReq[0] = '\0';

  result = inet_pton (AF_INET, argv[1], &sAddr.sin_addr); 
  sAddr.sin_family = AF_INET; 
  sAddr.sin_port = htons (HTTP_PORT); 

  socketfd = socket (AF_INET, SOCK_STREAM, 0);

  if (-1 == socketfd)
  { 
    fprintf (stderr, "No socket created\n\n");
    return EXIT_FAILURE;
  }

  result = (connect (socketfd, (struct sockaddr *) &sAddr, 
            sizeof (struct sockaddr_in)));

  if (-1 == result) 
  {
    perror ("Connection failed\n\n");
    close (socketfd);
    exit (EXIT_FAILURE);                   
  }

  printf (">|");
  snprintf (szGetReq, MAX_SIZE,"GET %s HTTP/1.1\r\nHost:" 
            "%s\r\nConnection: Close\r\n\r\n", argv[2], argv[1]);
  printf ("%s|<\n\n>|",szGetReq); 

  result = send (socketfd, szGetReq, MAX_SIZE, 0);

  if (-1 == result)
  {
    perror ("Failed to Send\n\n");
    close (socketfd);
    exit (EXIT_FAILURE);
  }

  result = recv (socketfd, szResponse, MAX_SIZE - 1, 0); 
  
  if (-1 == result)
  {
    perror ("Failed to Recieve\n\n");
    close (socketfd);
    exit (EXIT_FAILURE);
  }
  szResponse[result] = 0;
  printf ("%s|<\n",szResponse);

  close (socketfd);

  return EXIT_SUCCESS; // aspirational
}
