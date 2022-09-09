# Echo_Server_Lab

### Goal: Practice with TCP server sockets and Wireshark. Also to have fun.

In this lab you must write a local echo server using C sockets.  Your echo server must take, as a command line argument, the port on which to listen.  You must test your echo server with your previous TCP_Lab executable and telnet.


Read the file main.c  You will need to fill in code as described in each of the comments.  A rough outline of the code is below.  More instructions are listed in the source code:

    • Fill out the sockaddr_in struct, address family and port.  The port number will be in argv[1]. 
      The address should be INADDR_ANY.
    • Create a TCP socket. (socket)
    • Bind the socket to the port (bind)
    • Wait for an incoming connection.  Set the backlog to 10. (listen)
        ◦ Loop forever // while(1):
            ▪ Accept the incoming connection and get a new socket (accept)
            ▪ use inet_ntop() to get the connecting address into a printable format and display to the screen.
            ▪ Read and write the new socket once
            ▪ Close the new socket.
    • Close the original socket


Test your Echo Server by letting it run while you run your TCP_Lab multiple times in a row to connect.

### Running your Echo Server

#bin/main Port

bin/main 36001
