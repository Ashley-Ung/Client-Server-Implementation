//***************************************************************************
// File name:		httpget_large.c
// Author:			Ashley Ung
// Date:				September 30, 2021
// Purpose:			A program that uses TCP sockets to produce HTTP requests and 
//              displays the HTTP responses, which requires multiple reads. 
//              The content-length header will be parsed in order to 
//              determine the size of the response. 
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
#include <ctype.h>

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
  const int HTTP_END_MARKER = 4;
  const int MAX_CONNECTION = 11;
  
  int socketfd; 
  int result;
  int totalBytesRecv = 0; 
  int currentBytesRecv = 0; 
  int totalResponseSize = 0;
  int totalHeaderSize = 0;
  int contentLength; 
  char szGetReq[MAX_SIZE];
  char szResponse[MAX_SIZE];
  char tempChar;
  char *pHeader;
  char *pContent; 
  char *pEnd; 
  struct sockaddr_in sAddr;

  szGetReq[0] = '\0';

  result = inet_pton (AF_INET, argv[1], &sAddr.sin_addr); 
  sAddr.sin_family = AF_INET; 
  sAddr.sin_port = htons (HTTP_PORT); 

  socketfd = socket (AF_INET, SOCK_STREAM, 0);

  if (inet_pton (AF_INET, argv[1], &sAddr.sin_addr) < 0) 
  {
    perror ("Address in use or Invalid");
    return EXIT_FAILURE; 
  }

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
  
  for (int i = 0; i < MAX_CONNECTION; i++)                       
  {
    sleep (1);
    printf ("\n>|");
    snprintf (szGetReq, MAX_SIZE,"GET %s HTTP/1.1\r\nHost:" 
              "%s\r\nConnection: keep-alive\r\n\r\n", argv[2], argv[1]);  
    printf ("%s|<\n",szGetReq); 
    result = send (socketfd, szGetReq, strlen (szGetReq), 0);

    if (-1 == result)
    {
      perror ("Failed to Send\n\n");
      close (socketfd);
      exit (EXIT_FAILURE);
    }

    memset (szResponse, '\0', MAX_SIZE);                                    
    currentBytesRecv = recv (socketfd, szResponse, MAX_SIZE, 0);
    printf ("\nrecv ()\n");
    totalBytesRecv += currentBytesRecv;

    pHeader = strstr (szResponse,"\r\n\r\n"); 
    if (NULL == pHeader)                         
    {
      printf ("ERROR: \\r\\n\\r\\n: NOT FOUND!\n\n");
    }

    totalHeaderSize = pHeader - szResponse + HTTP_END_MARKER; 
  
    pContent = strstr (szResponse, "Content-Length: "); 

    if (NULL == pContent)                        
    {
      printf ("ERROR: Content: NOT FOUND!\n\n");
    }

    while (!isdigit (*pContent))
    {
      ++pContent;
    }

    pEnd = pContent;

    while (isdigit (*pEnd))
    {
      ++pEnd;
    }

    tempChar = *pEnd; 
    *pEnd = '\0'; 

    contentLength = atoi (pContent);  
    *pEnd = tempChar; 

    totalResponseSize = contentLength - totalHeaderSize;

    while (totalResponseSize > totalBytesRecv)
    {
      currentBytesRecv = (recv (socketfd, szResponse, MAX_SIZE, 0));
      printf ("recv ()\n");
      totalBytesRecv += currentBytesRecv;
    }       
    printf ("\nTotal Bytes Read:%d\n\n", totalBytesRecv);
  
    if (-1 == currentBytesRecv)  
    {
      perror ("Failed to Recieve\n\n");
      close (socketfd);
      exit (EXIT_FAILURE);
    }
    totalBytesRecv = 0;
    szResponse[result] = 0;
  }
  close (socketfd);

  return EXIT_SUCCESS;
} 
