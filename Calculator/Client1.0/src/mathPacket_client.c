//***************************************************************************
// File name:	  mathPacket_client.c
// Author:			Ashley Ung
// Date:				October 14, 2021
// Class:				CS 360
// Assignment:	Lab Math Packet 
// Purpose:		  A math packet client that builds a math packet request, sends 
//              sends the request to the server, parses and displays the 
//              result.
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
 Function:		parseErrorResponse
 
 Description: A helper function that parses the response message and response 
              code when the response code is an error code.
 
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
  printf ("\nResponse Code: %s\n", pResponseCode);  
  *pResponseEnd = tempChar;

  pResponseEnd++;
  pTemp = pResponseEnd;
  printf ("Response Message: "); 
  while (*pTemp != '\n')                    
  {
    printf ("%c", *pTemp);
    pTemp++;
  }
  printf ("\n\n");
} 

/****************************************************************************
 Function:		parseResponse
 
 Description: A helper function that parses the response message and response 
              code.
 
 Parameters:  szResponse - a storage buffer that stores the read in data. 
 
 Returned:		none
****************************************************************************/

void parseResponse (char *szResponse) 
{
  const int MAX_PACKET_SIZE = 1024;
  const char *VALID_RESPONSE_CODE = "100 OK";

  int byteDifference;
  char *pResponseCode; 
  char *pNewLine; 
  char responseClipped[MAX_PACKET_SIZE];  

  pResponseCode = strstr (szResponse, " ") + 1; 
  memset (responseClipped, '\0', MAX_PACKET_SIZE);
  pNewLine = strstr (szResponse, "\n");
  byteDifference  = pNewLine - pResponseCode;
  strncpy (responseClipped, pResponseCode, byteDifference);
  if (!strcmp (responseClipped, VALID_RESPONSE_CODE) == 0)
  {
    parseErrorResponse (szResponse);
  }
  else 
  {
    printf ("\nResponse Code: %s\n", responseClipped); 
  }  
}

/****************************************************************************
 Function:		parseResult
 
 Description: A helper function that parses the result.
 
 Parameters:  szResponse - a storage buffer that stores the read in data.
              socketfd   - the listening socket descriptor
 
 Returned:		none
****************************************************************************/

void parseResult (char *szResponse, int socketfd) 
{
  char tempChar;
  char *pResult;                                        
  char *pResultEnd;  

  pResult = strstr (szResponse, "Result: ");  
  if (NULL == pResult)
  {
    close (socketfd);
    exit (EXIT_FAILURE);
  }
  while (!isdigit (*pResult))
  {
    ++pResult;
  }
  pResultEnd = pResult; 
  while (isdigit (*pResultEnd))
  {
    ++pResultEnd;
  }
  tempChar = *pResultEnd;
  *pResultEnd = '\0';
  printf ("Result: %s\n", pResult);  
  *pResultEnd = tempChar;
}

/****************************************************************************
 Function:		parseRounding
 
 Description: A helper function that parses and prints the rounding status.
 
 Parameters:  szResponse - a storage buffer that stores the read in data.
              socketfd   - the listening socket descriptor
 
 Returned:		none
****************************************************************************/

void parseRounding (char *szResponse, int socketfd)
{
  const char *ROUNDING = "Rounding: ";

  char *pRounding; 
  
  pRounding = strstr (szResponse, "Rounding: ");
  if (NULL == pRounding)
  {
    close (socketfd);
    exit (EXIT_FAILURE);
  }
  pRounding += strlen (ROUNDING);       
  if (*pRounding == 'T')                                       
  {
    printf ("Rounded!\n");
  }
  else 
  {
    printf ("No Rounding\n");
  } 
}    

/****************************************************************************
 Function:		parseOverflow
 
 Description: A helper function that parses and prints the overflow status.
 
 Parameters:  szResponse - a storage buffer that stores the read in data.
              socketfd   - the listening socket descriptor
 
 Returned:		none
****************************************************************************/

void parseOverflow (char *szResponse, int socketfd)
{
  const char *OVERFLOW = "Overflow: ";

  char *pOverflow; 

  pOverflow = strstr (szResponse, "Overflow: "); 
  if (NULL == pOverflow)
  {
    close (socketfd);
    exit (EXIT_FAILURE);
  }
  pOverflow += strlen (OVERFLOW);
  if (*pOverflow == 'T')                                       
  {
    printf ("Overflow!\n");                                       
  }
  else 
  {
    printf ("No Overflow\n");
  }
}

/****************************************************************************
 Function:		parseXString
 
 Description: A helper function that parses and prints the XString if the 
              math packet contains an X-String. 
 
 Parameters:  szResponse - a storage buffer that stores the read in data. 
 
 Returned:		none
****************************************************************************/

void parseXString (char *szResponse, int socketfd) 
{
  char *pXString; 
  char *pTemp;     

  pXString = szResponse;
  while (*pXString != '\0')                                     
  {
    pXString = strstr (pXString, "X-");
  
    if (NULL == pXString)                                 
    {
      printf ("\n");
      close (socketfd);
      exit (EXIT_FAILURE);
    }
    
    pTemp = pXString;
    while (*pTemp != '\n')
    {
      printf ("%c", *pTemp); 
      pTemp++;
    }
    printf ("\n");
    pXString++;
  } 
}    
  
/****************************************************************************
 Function:		main
 
 Description:	Uses TCP sockets to produce MathPacket requests and displays 
              the repsonses. 
 
 Parameters:	int argc     - number of command line arguments
							char **argv  - the command line arguments
 
 Returned:		EXIT_SUCCESS
****************************************************************************/

int main (int argc, char **argv)
{ 
  const int MAX_PACKET_SIZE = 1024;
  const int HTTP_PORT = 8080;

  int socketfd; 
  int result;
  int option;
  struct sockaddr_in sAddr;
  char szGetReq[MAX_PACKET_SIZE];
  char szResponse[MAX_PACKET_SIZE];
  char szReadBuffer[MAX_PACKET_SIZE];                                                                                                           

  szGetReq[0] = '\0';
  szResponse[0] = '\0';
  szReadBuffer[0] = '\0';

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
                                           
  snprintf (szGetReq, MAX_PACKET_SIZE,"CALCULATE MATH/1.0\nOperand1: "
            "%s\nOperator: %s\nOperand2: %s\nConnection: Close\n\n", 
            argv[3], argv[4], argv[5]);  

  while ((option = (getopt (argc, argv, "-d"))) != -1)             
  {
    switch (option)
    {
      case 'd':
      printf (szGetReq);
      break;
    }
  }

  result = send (socketfd, szGetReq, strlen (szGetReq), 0);

  if (-1 == result)
  {
    perror ("Failed to Send\n\n");
    close (socketfd);
    exit (EXIT_FAILURE);
  }

  receivePacket (socketfd, szResponse, szReadBuffer);
  parseResponse (szResponse);                               
  parseResult (szResponse, socketfd);
  parseRounding (szResponse, socketfd);
  parseOverflow (szResponse, socketfd);
  parseXString (szResponse, socketfd);

  close (socketfd);

  return EXIT_SUCCESS;
} 