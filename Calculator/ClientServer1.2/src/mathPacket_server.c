//***************************************************************************
// File name:	  mathPacket_server.c
// Author:			Ashley Ung
// Date:			  11.12.2021 
// Purpose:		  A math packet server that uses TCP sockets to respond to
//              MathPacket requests. 
//***************************************************************************

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
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
 
 Parameters:  szRequest    - a storage buffer that stores the read in data. 
              szReadBuffer - a read buffer that stores the most current 
                             read in data.
              socketfd     - the listening socket descriptor
 
 Returned:		none
****************************************************************************/

void receivePacket (int socketfd, char *szRequest, char *szReadBuffer)
{
  const int MAX_PACKET_SIZE = 1024;
  
  char *pResponse;
    
  memset (szRequest, '\0', MAX_PACKET_SIZE); 
  memset (szReadBuffer, '\0', MAX_PACKET_SIZE); 

  pResponse = NULL;                                           

  while (!pResponse)  
  {
    recv (socketfd, szReadBuffer, MAX_PACKET_SIZE, 0);  
    strncat (szRequest, szReadBuffer, MAX_PACKET_SIZE - 
             strlen (szRequest) - 1);
    memset (szReadBuffer, '\0', MAX_PACKET_SIZE);
    pResponse = strstr (szRequest, "\n\n");
  } 
}

/****************************************************************************
 Function:		parseRequest 

 Description: A helper function that parses the passed in packet. 
 
 Parameters:  szRequest  - a storage buffer that stores the read in data
              szBuffer   - a copy of the string to search for 
              pSearch    - the string to search for 
 
 Returned:		none
****************************************************************************/

void parseRequest (char *szRequest , char *szBuffer, char *pSearch) 
{
  char *pEnd, *pFind; 

  pFind = strstr (szRequest, pSearch);

  pFind += strlen (pSearch);
  pEnd = pFind;
  while (*pEnd != '\n')
  {
    pEnd++;
  }
  strncpy (szBuffer, pFind, pEnd - pFind);                                              
}

/**************************************************************************** 
 Function:		calculate
 
 Description: A helper function that evaluates the operators and operands 
              to perform the correct arithmetic
 
 Parameters:  szRequest      - a storage buffer that stores the read in data
              operandOne     - the first operand to evaluate 
              operandTwo     - the second operand to evaluate 
              operator       - the operator used to evaluate the operands
              bIsRounded     - a boolean to track if the result is rounded
              bIsBadOperator - a boolean to track if the operator is bad
 
 Returned:		The calculated result.
****************************************************************************/

int calculate (char *szRequest, int operandOne, int operandTwo, 
               char operator, bool *bIsRounded, bool *bIsBadOprator)
{
  int result = 0;

  if (operator == '+')
  {
    result = operandOne + operandTwo; 
  }
  else if (operator == '-')
  {
    result = operandOne - operandTwo;
  }
  else if (operator == 'x')
  {
    result = operandOne * operandTwo; 
  }
  else if (operator == '/')               
  {
    result = operandOne / operandTwo;                               
    if (operandOne % operandTwo != 0)
    {
      *bIsRounded = true;
    }
  }
  else if (operator == '%')
  {
   result = operandOne % operandTwo; 
  }   
  else                                                               
  {
    *bIsBadOprator = true;
  }                                                                
  return result;
}

/****************************************************************************
 Function:		main
 
 Description:	Uses TCP sockets to respond to MathPacket requests and displays 
              the requests. 
 
 Parameters:	int argc     - number of command line arguments
							char argv    - the command line arguments
 
 Returned:		EXIT_SUCCESS
****************************************************************************/

int main (int argc, char **argv)
{
  const int MAX_PACKET_SIZE = 1024;   
  const char *CALCULATE = "CALCULATE ", *CONTINUE = "CONTINUE ";                                                                                                                           
  
  int socketfd, connectSocket, result;
  struct sockaddr_in sAddr, sConnAddr;                      
  char szRequest[MAX_PACKET_SIZE];
  char szReadBuffer[MAX_PACKET_SIZE];    
  char szBuffer[MAX_PACKET_SIZE];  
  char szConnection[MAX_PACKET_SIZE];
  char szSendBuffer[MAX_PACKET_SIZE];                               
  char szRounded[MAX_PACKET_SIZE];
  char szHeader[MAX_PACKET_SIZE];   
  int  operandOne, operandTwo, value;     
  int currentValue = 0;
  char operator;   
  char *pCalculate;
  char *pContinue; 
  bool bIsCalculate = false, bIsContinue = false, bIsRounded = false; 
  bool bIsBadOprator = false;                                                                          

  memset (szBuffer, '\0', MAX_PACKET_SIZE); 
  memset (szSendBuffer, '\0', MAX_PACKET_SIZE); 
  memset (szRounded, '\0', MAX_PACKET_SIZE); 
  memset (szHeader, '\0', MAX_PACKET_SIZE);

  socklen_t addrLen = sizeof (struct sockaddr_in);                     

  socketfd = socket (AF_INET, SOCK_STREAM, 0);      
  if (-1 == socketfd)
  { 
    fprintf (stderr, "No socket created\n\n");
    return EXIT_FAILURE;
  }

	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons (atoi (argv[1])); 
	sAddr.sin_addr.s_addr = htonl (INADDR_ANY); 

	result = bind (socketfd, &sAddr, sizeof (struct sockaddr_in));                                                                                
  while (-1 == result)
  {
    perror ("Bind");  
    printf ("Unable to bind to the port.");
    sleep (30);
    result = bind (socketfd, &sAddr, sizeof (struct sockaddr_in));
  }

	result = listen (socketfd, 10);                                  
	if (-1 == result) 
	{
		perror ("listen");
		close (socketfd);
		return EXIT_FAILURE;
	}

  while (1) 
  {
    connectSocket = accept (socketfd, &sConnAddr, &addrLen);
    if (-1 == connectSocket)
    {
      perror ("Socket not connected");
      close (connectSocket);
      return EXIT_FAILURE;
    }

    printf ("Incoming\n");    
    receivePacket (connectSocket, szRequest, szReadBuffer);
    printf ("%s", szRequest);
                                         
    parseRequest (szRequest, szHeader, "MATH/");                     
    if (strcmp (szHeader, "1.1"))
    {
      snprintf (szSendBuffer, MAX_PACKET_SIZE,"MATH/%s 500 OlderCodeDoesNot"
                "Checkout\nConnection: Close\n\n", szHeader);
      result = send (connectSocket, szSendBuffer, strlen (szSendBuffer),    
                     MSG_NOSIGNAL); 
    }
    else 
    {                                                               
      memset (szConnection, '\0', MAX_PACKET_SIZE);                             
      parseRequest (szRequest, szConnection, "Connection: ");
      memset (szBuffer, '\0', MAX_PACKET_SIZE);
      
      pCalculate = strstr (szRequest, CALCULATE);                      
      if (NULL != pCalculate)
      {
        bIsCalculate = true; 
      }                              
      if (bIsCalculate == true)                                   
      {
        parseRequest (szRequest, szBuffer, "Operand1: ");
        operandOne = atoi (szBuffer);
        memset (szBuffer, '\0', MAX_PACKET_SIZE); 
        parseRequest (szRequest, szBuffer, "Operator: ");
        operator = szBuffer[0]; 
        memset (szBuffer, '\0', MAX_PACKET_SIZE);
        parseRequest (szRequest, szBuffer, "Operand2: ");      
        operandTwo = atoi (szBuffer);                      
        memset (szBuffer, '\0', MAX_PACKET_SIZE);
        value = calculate (szRequest, operandOne, operandTwo, operator, 
                           &bIsRounded, &bIsBadOprator);
        currentValue = value;
        if (bIsBadOprator)                                                  
        {
          snprintf (szSendBuffer, MAX_PACKET_SIZE,"MATH/1.1 200 BAD_OPERATOR" 
                    "\nConnection: Close\n\n");
          result = send (connectSocket, szSendBuffer, strlen (szSendBuffer),    
                        MSG_NOSIGNAL); 
        }
        else
        {
          if (bIsRounded)
          {
            snprintf (szRounded, MAX_PACKET_SIZE, "%s", "True");
          }
          else 
          {
            snprintf (szRounded, MAX_PACKET_SIZE, "%s", "False");
          }
          printf ("Outgoing\n");          
          snprintf (szSendBuffer, MAX_PACKET_SIZE,"MATH/1.1 100 OK\nResult: "     
                    "%d\nRounding: %s\nOverflow: False\nX-Server-Version: "
                    "1.1.0\nConnection: %s\n\n", value, szRounded, 
                    szConnection);   
          printf (szSendBuffer);
          result = send (connectSocket, szSendBuffer, strlen (szSendBuffer),    
                         MSG_NOSIGNAL);                       
        } 
      }                                
      
      while (!strcmp (szConnection, "Keep-Alive"))
      {
        printf ("Incoming\n");
        receivePacket (connectSocket, szRequest, szReadBuffer); 
        printf ("%s", szRequest);                                        
        pContinue = strstr (szRequest, CONTINUE);
        memset (szConnection, '\0', MAX_PACKET_SIZE); 
        parseRequest (szRequest, szConnection, "Connection: ");
        if (NULL != pContinue)                                                          
        {
          bIsContinue = true;
        }
        if (bIsContinue == true)                                    
        {        
          parseRequest (szRequest, szBuffer, "Operator: ");
          operator = szBuffer[0]; 
          memset (szBuffer, '\0', MAX_PACKET_SIZE);
          parseRequest (szRequest, szBuffer, "Operand2: ");      
          operandTwo = atoi (szBuffer);                      
          memset (szBuffer, '\0', MAX_PACKET_SIZE);
          value = calculate (szRequest, currentValue, operandTwo, operator, 
                             &bIsRounded, &bIsBadOprator);
          currentValue = value; 
          if (bIsBadOprator)                                                 
          {
            snprintf (szSendBuffer, MAX_PACKET_SIZE,"MATH/1.1 200 " 
                      "BAD_OPERATOR\nConnection: Close\n\n");
            result = send (connectSocket, szSendBuffer, 
                           strlen (szSendBuffer), MSG_NOSIGNAL); 
          }
          else
          {
            if (bIsRounded)
            {
              snprintf (szRounded, MAX_PACKET_SIZE, "%s", "True");
            }
            else 
            {
              snprintf (szRounded, MAX_PACKET_SIZE, "%s", "False");
            }
            printf ("Outgoing\n");          
            snprintf (szSendBuffer, MAX_PACKET_SIZE,"MATH/1.1 100 OK\n"     
                      "Result: %d\nRounding: %s\nOverflow: False\n"
                      "X-Server-Version: 1.1.0\nConnection: %s\n\n", value, 
                      szRounded, szConnection);   
            printf (szSendBuffer);
            result = send (connectSocket, szSendBuffer, 
                           strlen (szSendBuffer), MSG_NOSIGNAL);                              
          }        
        }
      }                                          
      close (connectSocket);
    }
  }
  close (socketfd);
  return EXIT_SUCCESS;
}
