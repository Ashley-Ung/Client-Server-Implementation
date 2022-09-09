# TCP_Lab_Student

Submit your answers to Moodle for questions 0-6 by Thursday, September 23, 2:45 pm. 

Goal: Practice with TCP sockets, Wireshark, and ncat.

In this lab you must start a local echo server using ncat and write a C program that will connect to the echo server using a TCP socket and send and receive one four byte integer.  The four byte integer must be sent and received in network byte order. 

To run the echo server on the VM:

	ncat -l 36001 --keep-open --exec "/bin/cat"

To test the echo server:
	Open a new terminal window and connect to the echo server using telnet:

	telnet localhost 36001

	Type any message and see that message echoed back to you. 

To write the C code:

	Accept the assignment from GitHub Classroom.  
	Use git clone to bring the project down to your VM.
	Open the folder in VS Code
	Edit the file src/TCP_Lab.c by filling in the missing code.

	To build the executable:
	make

	To run the executable and connect to your running ncat server on the local VM:

     # bin/TCP_Lab HostIP Port
	bin/TCP_Lab 127.0.0.1 36001

Read the file TCP_Lab.c  You will need to fill in code as described in each of the comments.  A rough outline of the code is below.  More instructions are listed in the source code:

    * Use inet_pton() to turn argv[1] from a text representation to an address in network-order, 
      binary format in sockaddr_in.sin_addr and fill out the rest of the sockaddr_in struct.  
      The port number will be in argv[2]
    * Create a TCP socket. (socket)
    * Put the integer (value) into network byte order (htonl)
    * Create a connection to the echo server (connect)
    * Send the integer to the network (send)
    * Read the integer from the network (recv)
    * Put the integer into host byte order (ntohl)
    * Close the socket (close)

An example run, redacted of vital information, is at the end of this document.

Wireshark

Open wireshark and start capturing packets on interface any.  Filter the packets by the source and destination port as follows:

tcp.dstport == 36001 || tcp.srcport == 36001

This will show you any packets that have 36001 as either the source or destination port.  Be sure the SPort column is displayed in wireshark.  This column will help you to identify if the packet is originating from your code or from the echo server.

Run your code once.  Stop the packet capture.
	
	0. How is 42 represented in hexadecimal?

	1. How many packets meet the filter criteria?

	2. What SPort indicates the packet was sent from your C code?

	3. What SPort indicates the packet was sent from the echo server?

	4. Which packet (first, second, third?)  sends the four byte integer from the client to the echo server?

	5. Which packet (first, second, third?) sends the four byte integer from the echo server to the client?

	6. How many total bytes of network traffic are generated to send these eight bytes of data?

Example Run:

chadd@:black_large_square::black_large_square::black_large_square::~/TCP_Lab> bin/TCP_Lab 127.0.0.1 36021

42

Network Byte Order (send): :black_large_square::black_large_square::black_large_square::black_large_square:

Network Byte Order (receive): :black_large_square::black_large_square::black_large_square::black_large_square:

42

chadd@:black_large_square::black_large_square::black_large_square::black_large_square::~/TCP_Lab>
