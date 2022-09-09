# Lab_HTTP
httpget

### Objective:

For this lab, you will write a C program that will use TCP sockets to produce HTTP requests and display the HTTP responses.  You will also use Wireshark to view the HTTP traffic and debug your program.

### Problem:

You must write a program, httpget, that will take exactly two command line arguments, a web server address and a path to a web page.  

	Build an HTTP request using the command line arguments
	Display the HTTP request (printf)
	Send the request to the server
	Display (printf) the resulting HTTP response.  

An example run in shown in this document with some data redacted.  

You will need to submit both your source code (via GitHub) and a written lab report (PDF, submitted to Moodle) providing the data you generate and the answer to discussion questions.  Answer the questions in complete, well written sentences.  The discussion questions may have answers in the notes, in the textbook, or require you to do research online. 

Start early!

### Steps:

* Open Wireshark, and start capturing http traffic.

* Open Firefox (so we all get the same results) and clear your cache (search for "Clear Data" in your Firefox Settings).

* Load the page:

	http://64.59.225.81/chadd/cs360f21/LabHttp.html

* In Wireshark, find the HTTP message that GETs the page and the HTTP response that returns the page.  Take a screenshot of each Hypertext Transfer Protocol message and paste them into your document.  Be sure to clearly label each image.  Be sure the HTTP data is clearly visible in the middle pane.

Based on the packet captured above, you must build an HTTP request message to send to the server.  The only required fields are the first line (GET) and HOST:   Add a Connection: close line to close the TCP connection after the response is provided.  Each line in the HTTP request message ends in \r\n and the entire HTTP request message is ended with another \r\n.

```
1. In your captured HTTP Request, what is the source port?

2. What is the content of the Connection: field in your captured HTTP request?  
   What does this mean?
       
3. Study the HTTP request.  What information does the server know about your computer? 

4. What HTTP code did you receive back from the server?

5. Study the HTTP response.  What information does your browser know about the server?
```
* Reload the page in the web browser and capture the new HTTP request and HTTP response.  Take a screenshot of each Hypertext Transfer Protocol message and paste them into your document.  Be sure to clearly label each image.  

```
6. How did the HTTP request change (what additional fields were provided)? 
   What does each of these fields mean?  
   Where did the values in these fields originate?

7. What HTTP code did you receive back from the server?

8. What causes the HTTP server to respond with this code?
```

* Use your httpget command to fetch the web page.  Be sure to use Wireshark to capture the HTTP request and HTTP response.  Take a screenshot of each Hypertext Transfer Protocol message and paste them into your document.  Be sure to clearly label each image.  Take a screenshot of the execution of your program and paste that into your document.  Be sure to run your program in a terminal with black text on a white background.

```
9.  How many bytes were on the wire for your HTTP request?

10. Does the ETag in this HTTP response match the ETag in the previous HTTP responses? 
    Why or why not?
```
* Restart the Wireshark capture.

* Load the following page into Firefox:

 * http://pubs.opengroup.org/onlinepubs/009696699/basedefs/sys/socket.h.html
```
11. How many HTTP requests are made to completely load this page?

12. Are any concurrent connections opened to the server to fetch objects in parallel?  
    If so, take a screenshot of the relevant portion of Wireshark and describe how 
    you recognized concurrent connections.

13. What types of data besides text/html are returned in individual HTTP responses?
```
### TCP Socket Programming with C

Your program will need to use the following C functions.  See the class notes on how to use each of these functions.

	int socket(int, int, int)
	int connect(int, struct sockaddr *, socklen_t )
	int send(int, void *, size_t, int)
	int recv(int, void*, size_t, int);
	close(int)

You can be guaranteed for this assignment that each HTTP message will be at most 4096 bytes.

### String Programming with C

You can use the function strncat(char *dest, char *src, size_t n) to concatenate two null terminated strings.  Be sure the dest string has plenty of space to add in the characters in src and the null terminator.  
snprintf() might also be useful.

Recall that memory is not zeroed out by default but you can create a statically allocated, null terminated string as follows:

```
const int MAX_SIZE = 4096;
char szGetReq[MAX_SIZE];
szGetReq[0] = '\0';
```

### Example Run	


you@linux# httpget 64.59.225.81 /chadd/cs360f21/LabHttp.html
```
>|GET


|<

>|HTTP/1.1 





</html>
|<
```
Note that for ease of reading, when printing the request and response the data must be surrounded by >| |<
