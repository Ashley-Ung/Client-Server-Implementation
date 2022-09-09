//***************************************************************************
// File name:		Echo_Server_Lab.c
// Author:			chadd williams
// Date:				September 10, 2018
// Purpose:			Practice using TCP server sockets
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
 
 Description:	Act as an echo server
 
 Parameters:	int argc: number of command line arguments
							char **argv: the command line arguments
 
 Returned:		EXIT_SUCCESS
****************************************************************************/
int main(int argc, char **argv)
{

		int socketfd, connSocket;
		int result;
		char pBuf[1024]; // a buffer that we will send back and forth 

		//to run this: bin/main 36001 
		//argv[0] = username 
		//argv[1] = our port 

		////
		struct sockaddr_in sAddr, sConnAddr; //local addr and addr of other end 
		socklen_t addrLen = sizeof(struct sockaddr_in);
		
		// create an AF_INET Stream socket.
		socketfd = socket(AF_INET, SOCK_STREAM, 0); //sock stream= a byte stream & use TCP

		if (-1 == socketfd) 
		{
			fprintf(stderr, "No socket\n\n");
			return EXIT_FAILURE;
		}
			
			
		// make sure the socket was created.
		// otherwise, exit witmah an error message.


		// set up the socket family and port in the address
		// where we are ;isting theres an address & a port. 
		//The listening address port 
		sAddr.sin_family = AF_INET;
		sAddr.sin_port = htonl(atoi(argv[1])); //port comes from command line 
		sAddr.sin_addr.s_addr = htonl(INADDR_ANY); //listen to any address, etho vs lo
	
	
		// bind socket to listen address
		result = bind (socketfd, &sAddr, sizeof(struct sockaddr_in));
		perror("Bind ");

		if (-1 == result) 
		{ 
			printf("BIND ERROR");
			close (socketfd);
			return EXIT_FAILURE;
		}

		// listen with backlog of 10
		result = listen (socketfd, 1); //chenage to 1 to see

		if (-1 == result) 
		{
			perror("listen");
			close (socketfd);
			return EXIT_FAILURE;
		}


		// loop forever
		while (1)
		{
			//accept connection 
			//src addr port & dest addr port 
			connSocket= accept(socketfd, &sConnAddr, &addrLen);

			if (-1 == connSocket) 		fprintf(stderr, "Recv: %d\n", result);
			{
				perror("accept");
				close(socketfd);
				return EXIT_FAILURE;
			}

			// receive data
			result = recv(connSocket, pBuf, 1024, 0);
			fprintf(stderr, "Recv: %d\n", result);   //stderr vs stout, stout could be buffered or delayed, 
																							 // standard error is quicker 

			// send data back
			result = send(connSocket, pBuf, result, 0);
			fprintf(stderr, "Recv: %d\n", result);

			// close the accepted socket
			close(connSocket);
		}
		// close the original socket
		close(socketfd); 

		return EXIT_SUCCESS;
}

