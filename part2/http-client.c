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
    char *url = argv[3];

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));     /* Zero out structure */
    servAddr.sin_family      = AF_INET;             /* Internet address family */
    servAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    servAddr.sin_port        = htons(servPort); /* Server port */

    /* Establish the connection to the server */
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("connect() failed");

    printf("HI");

    close(sock);
    exit(0);
}



