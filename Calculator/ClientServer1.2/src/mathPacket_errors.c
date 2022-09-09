//***************************************************************************
// File name:	  mathPacket_errors.c
// Author:			Ashley Ung
// Date:				October 14, 2021
// Purpose:		  A math packet error driver that causes each of the error 
//              codes (200-400) to be generated, in numeric order. 
//***************************************************************************

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
 Function:		parseErrorResponse
 
 Description: A helper function that parses the response message and response 
              code.
 
 Parameters:  szResponse - a storage buffer that stores the read in data. 
 
 Returned:		none
****************************************************************************/

void parseErrorResponse (char *szResponse)
{
  char *pResponseCode; 
  char tempChar;
  char *pResponseEnd;
  char *pTemp;

  pResponseCode = strstr (szResponse, " ") + 1; 

  while (!isdigit (*pResponseCode))
  {
    ++pResponseCode;
  }

  pResponseEnd = pResponseCode; 

  while (isdigit (*pResponseEnd))
  {
    ++pResponseEnd;
  }

  tempChar = *pResponseEnd;
  *pResponseEnd = '\0';
  printf ("Response Code: %s\n", pResponseCode);  
  *pResponseEnd = tempChar;

  pResponseEnd++;
  pTemp = pResponseEnd;
  printf ("Response Message: "); 
  while (*pTemp != '\n')                    
  {
    printf ("%c", *pTemp);
    pTemp++;
  }
  printf ("\n====\n\n");
} 

/****************************************************************************
 Function:		receivePacket
 
 Description: A helper function that receives the packet data and stores 
              the data into a storage buffer until the entire packet is 
              received.
 
 Parameters:  szResponse   - a storage buffer that stores the read in data. 
              szReadBuffer - a read buffer that stores the most current 
                             read in data.
 
 Returned:		none
****************************************************************************/

void receivePacket (int socketfd, char *szResponse, char *szReadBuffer)
{
  const int MAX_PACKET_SIZE = 1024;
  
  char *pResponse;
    
  memset (szResponse, '\0', MAX_PACKET_SIZE); 
  memset (szReadBuffer, '\0', MAX_PACKET_SIZE); 

  pResponse = NULL;                                           

  while (!pResponse)  
  {
    recv (socketfd, szReadBuffer, MAX_PACKET_SIZE, 0);  
    strncat (szResponse, szReadBuffer, MAX_PACKET_SIZE - 
             strlen (szResponse) - 1);
    memset (szReadBuffer, '\0', MAX_PACKET_SIZE);
    pResponse = strstr (szResponse, "\n\n");
    } 
}

/****************************************************************************
 Function:		validateSocket
 
 Description: Checks if the socket is created.
 
 Parameters:  socketfd - the listening socket descriptor
 
 Returned:	  EXIT_FAILURE
****************************************************************************/

int validateSocket (int socketfd) 
{
  if (-1 == socketfd)
  {
    fprintf (stderr, "No socket created\n\n");
  }
  return EXIT_FAILURE;
}

/****************************************************************************
 Function:		validateConnection
 
 Description: Checks if the connection between two sockets are established,
              if there is no connection, the socket closes. 
 
 Parameters:  socketfd - the listening socket descriptor
              result   - the connection result 
 
 Returned:		EXIT_FAILURE 
****************************************************************************/

int validateConnection (int socketfd, int result) 
{
  if (-1 == result) 
  {
    perror ("Connection failed\n\n");
    close (socketfd);                 
  }
  return EXIT_FAILURE;
}

/****************************************************************************
 Function:		validateSend
 
 Description: Checks if the data was sent, if the data failed to send, the 
              socket closes.
 
 Parameters:  socketfd - the listening socket descriptor
              result   - the send result 
 
 Returned:		EXIT_FAILURE
****************************************************************************/

int validateSend (int socketfd, int result)
{
  if (-1 == result)
  {
    perror ("Failed to Send\n\n");
    close (socketfd);
  }
  return EXIT_FAILURE;
}

/****************************************************************************
 Function:		validateAddress
 
 Description: Checks to see if the address is valid, if the address is not 
              valid, an error message is printed to the screen. 
 
 Parameters:  argv - the command line arguments 
 
 Returned:		EXIT_FAILURE
****************************************************************************/

int validateAddress (char **argv)
{
  struct sockaddr_in sAddr;
  
  if (inet_pton (AF_INET, argv[1], &sAddr.sin_addr) < 0) 
  {
    perror ("Address in use or Invalid");
  }
  return EXIT_FAILURE;
}

/****************************************************************************
 Function:	  generateBadOperatorError 
 
 Description: Generates error response code 200, a bad operator. 
 
 Parameters:  szGetReq - a buffer that stores the request
 
 Returned:		None
****************************************************************************/

void generateBadOperatorError (char *szGetReq) 
{
  const int MAX_PACKET_SIZE = 1024;

  printf ("====\nExpected Code: 200\n\n"); 
  snprintf (szGetReq, MAX_PACKET_SIZE,"CALCULATE MATH/1.1\nOperand1: "
            "1\nOperator: :-)\nOperand2: 2\nConnection: Close\n\n");  
  printf ("%s",szGetReq); 
}     

/****************************************************************************
 Function:	  generateOlderCodeDoesNotCheckoutError
 
 Description: Generates error response code 500, a math packet version that
              is not version 1.1.
 
 Parameters:  szGetReq - a buffer that stores the request
 
 Returned:		None
****************************************************************************/

void generateOlderCodeDoesNotCheckoutError (char *szGetReq) {
  const int MAX_PACKET_SIZE = 1024;

  printf ("====\nExpected Code: 500\n\n"); 
  snprintf (szGetReq, MAX_PACKET_SIZE,"CALCULATE MATH/1.0\nOperand1: "
            "1\nOperator: +\nOperand2: 4\nConnection: Close\n\n");  
  printf ("%s",szGetReq); 
 }

/****************************************************************************
 Function:		main
 
 Description:	Uses TCP sockets to produce MathPacket errors and causes each 
              of the error codes (200-400) to be generated in numeric order.
 
 Parameters:	int argc     - number of command line arguments
							char **argv  - the command line arguments
 
 Returned:		EXIT_SUCCESS
****************************************************************************/

int main (int argc, char **argv)
{
  const int MAX_PACKET_SIZE = 1024;

  int socketfd; 
  int result;   
  char szGetReq[MAX_PACKET_SIZE];
  char szResponse[MAX_PACKET_SIZE];
  char szReadBuffer[MAX_PACKET_SIZE]; 
  struct sockaddr_in sAddr;

  szGetReq[0] = '\0';
  szResponse[0] = '\0';
  szReadBuffer[0] = '\0';

  result = inet_pton (AF_INET, argv[1], &sAddr.sin_addr); 
  sAddr.sin_family = AF_INET; 
  sAddr.sin_port = htons (atoi(argv[2])); 

  socketfd = socket (AF_INET, SOCK_STREAM, 0);
  validateAddress (argv);
  validateSocket (socketfd);
  result = (connect (socketfd, (struct sockaddr *) &sAddr, 
            sizeof (struct sockaddr_in)));
  validateConnection (socketfd, result);                             
  generateBadOperatorError (szGetReq);
  result = send (socketfd, szGetReq, strlen (szGetReq), 0);
  validateSend (socketfd, result);
  receivePacket (socketfd, szResponse, szReadBuffer);
  parseErrorResponse (szResponse);          
  close (socketfd);

  socketfd = socket (AF_INET, SOCK_STREAM, 0);
  validateAddress (argv);
  validateSocket (socketfd);
  result = (connect (socketfd, (struct sockaddr *) &sAddr, 
            sizeof (struct sockaddr_in)));
  validateConnection (socketfd, result);
  generateOlderCodeDoesNotCheckoutError (szGetReq);
  result = send (socketfd, szGetReq, strlen (szGetReq), 0);
  validateSend (socketfd, result);
  receivePacket (socketfd, szResponse, szReadBuffer);
  parseErrorResponse (szResponse);            
  close (socketfd);

  return EXIT_SUCCESS;
}
