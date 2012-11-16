//Emily Schultz, ess2183
//CS3157, Lab6, Part2
// Modified original code from
// http://cs.baylor.edu/~donahoo/practical/CSockets/textcode.html

#include <sys/types.h>
#include <netdb.h>

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

// Error handling function
void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in servAddr;     /* server address */
    unsigned short servPort;         /* server port */

    if (argc != 4)    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Hostname> <Port> <URL>\n", argv[0]);
       exit(1);
    }
    
    char *serverName = argv[1];

    // convert hostname into an IP address
    struct hostent *he;
    if((he = gethostbyname(serverName)) == NULL)
	DieWithError("gothostbyname failed");

    // server IP address
    char *servIP = inet_ntoa(*(struct in_addr *)he->h_addr);

    // Port Number
    servPort = atoi(argv[2]);

    // rest of the URL
    char *path = argv[3];

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
    servAddr.sin_family      = AF_INET;        /* Internet address family */
    servAddr.sin_addr.s_addr = inet_addr(servIP);    /* Server IP address */
    servAddr.sin_port        = htons(servPort);    /* Server port */

    /* Establish the connection to the server */
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("connect() failed");

    // HTTP GET request string
    char request[1000];
    strncpy(request, "GET ", sizeof(request));
    strcat(request, path);
    strcat(request, " HTTP/1.0\n");
    strcat(request, "Host: ");
    strcat(request, serverName);
    strcat(request, ":");
    strcat(request, argv[2]);
    strcat(request, "\r\n\r\n");
    
    // send the HTTP GET request
    send(sock, request, strlen(request), 0);

    // use fdopen to wrap socket with FILE* to read
    FILE *read = fdopen(sock, "r");

    // get the filename from the end of the path
    FILE *out = fopen(strrchr(path, '/')+1, "w");

    char line[100000];
    int linenum = 0;
    int pastheaders = 0;
    
    //read the response
    while(fgets(line, sizeof(line), read) != NULL)
      {
	if(linenum == 0)
	  {
	    // if the first line is not 200, print line and exit
	    if(strstr(line, "200") == NULL)
	      {
		printf("%s", line);
		fclose(read);
		close(sock);
		exit(1);
	      }
	    linenum = linenum + 1;
	  }
	else
	  {
	    // once beyond the headers, print to the file
	    if(pastheaders == 1)
		fprintf(out, "%s", line);
	    // blank line ends all the headers
	    if(strcmp(line,"\r\n") == 0 && pastheaders == 0)
		pastheaders = 1;
	  }
      }
    // close the socket, the send file, and the output file
    fclose(out);
    fclose(read);
    close(sock);
    exit(0);
}



