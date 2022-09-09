//TCP Sockets 

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



// wireshark
// tcp.dstport == 36001 || tcp.srcport == 36001

// start the echo server
// ncat -l 36001 --keep-open --exec "/bin/cat" # tcp
// /usr/sbin/ss -l -t -4 -p

// run your code
// bin/TCP_Lab 127.0.0.1 36001

// alternative nc commands to run the echo server:
//  nc -l 36001
//  nc 127.0.0.1 36001


/****************************************************************************
 Function:		main
 
 Description:	send a 4 byte integer to an echo server and display the returned
							4 byte int
 
 Parameters:	int argc: number of command line arguments
							char **argv: the command line arguments
 
 Returned:		EXIT_SUCCESS
****************************************************************************/
int main(int argc, char **argv)
{

	int socketfd;
	int value = 42;
	int result;
	char *pBuf = (char*) &value;
	struct sockaddr_in sAddr;
		
		
	// use inet_pton() to transform argv[2],
	// a text representation of an IPv4 address,
	// to a network-order, binary representation
	// stored in sAddr.sin_addr
		
	// be sure to the family and port in 
	// sAddr.

	result = inet_pton(AF_INET, argv[1], &sAddr.sin_addr); //set IP address
	sAddr.sin_family = AF_INET; // set family 
	sAddr.sin_port = htons(atoi(argv[2])); // convert port over to network bytes
	
		
	// create an AF_INET Stream socket.
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
			
			
	// make sure the socket was created.
	// otherwise, exit with an error message.
	if (-1 == socketfd)
	{
		fprintf(stderr, "no socket");
	}
		
	// display the integer in value
	printf("%d\n\n", value);
	
	// convert value to network-order
	value = htonl(value);

	// display the integer in value, now in network order
	printf("Network Byte Order (send): %x\n\n", value);

	// create a connection
	result = connect(socketfd, (struct sockaddr *) &sAddr, sizeof(struct sockaddr_in));

	//send
	result = send (socketfd, pBuf, sizeof(int), 0); //we know that int is 4 bytes long or use sizeof9)
																									//pBuf points to the first byte of value 

	// reset the value to 0
	value = 0;
		
	// recv a 4 byte int from the socket.
	result = recv(socketfd, pBuf, sizeof(int), MSG_WAITALL); // we will wait for all 4 bytes to come back

	// make sure all 4 bytes were read
	// otherwise, display an error message
	if (sizeof(int) != result)
	{
		printf("Incomplete read: %d\n\n", result);
	}

	// display the integer in value, now in network order
	printf("Network Byte Order (receive): %x\n\n", value);


	// convert value to host-order
	value = ntohl(value);
		
 	// display the integer in value
	printf("%d\n\n", value);
	
	
	// close the socket.
	close(socketfd);
		
	return EXIT_SUCCESS;
}

//our server is netcad
