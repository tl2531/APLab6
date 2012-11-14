// Emily Schultz, ess2183 Lab6, part1
// Modified originial code from:
// http://cs.baylor.edu/~donahoo/practical/CSockets/textcode.html
// and Jae's solution code for Lab4,Part1

// include mdb.h, mylist.h ??
#include "mdb.h"
#include "mylist.h"

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define MAXPENDING 5    /* Maximum outstanding connection requests */
#define KeyMax 5        /* Maximum characters to use from input for dblookup*/

// Error Handling function
void DieWithError(char *errorMessage)
{
  perror(errorMessage);
  exit(1);
}
// TCP client handling function
void HandleTCPClient(int clntSocket, char *filename)
{
  // open the socket to get input
  FILE *input = fdopen(clntSocket, "r");
  
  // open the database file specified in the command line
  FILE *fp = fopen(filename, "r");
  
  if(fp == NULL)
    DieWithError(filename);

  // read all records into memory
  struct List list;
  initList(&list);
  struct MdbRec r;
  struct Node *node = NULL;

  while(fread(&r, sizeof(r), 1, fp) == 1)
    {
      // allocate memory for a new record and copy into it
      // the one that was just read from database
      struct MdbRec *rec = (struct MdbRec *)malloc(sizeof(r));
      if(!rec)
	DieWithError("malloc failed");
      memcpy(rec, &r, sizeof(r));

      //add the record to the linked list
      node = addAfter(&list, node, rec);
      if(node == NULL)
	DieWithError("addAfter() failed");
    }

  // see if fread produced error
  if(ferror(fp))
    DieWithError(filename);

  //lookup loop
  char line[1000];
  char key[KeyMax + 1];
  
  while(fgets(line, sizeof(line), input) != NULL)
    {
      // must null-terminate the string manually after strncpy()
      strncpy(key, line, sizeof(key)-1);
      key[sizeof(key)-1] = '\0';
      // if newline is there, remove it
      size_t last = strlen(key) - 1;
      if(key[last] == '\n')
	key[last] = '\0';

      // traverse the list, printing out the matching records
      struct Node *node = list.head;
      int recNo = 1;
      while(node)
	{
	  struct MdbRec *rec = (struct MdbRec *)node->data;
	  if(strstr(rec->name, key) || strstr(rec->msg,key))
	    {
	      char output[18 + sizeof(struct MdbRec)];
	      sprintf(output, "%4d: {%s} said {%s}\n", recNo, rec->name, rec->msg);
	      send(clntSocket, output, strlen(output),0);
	    }
	  node = node->next;
	  recNo++;
	}
      send(clntSocket, "\n", strlen("\n"), 0); 
    }
  // clean up and quit
  
  // free all the records
  traverseList(&list, &free);
  removeAllNodes(&list);
  fclose(fp);
  fclose(input);
}

int main(int argc, char *argv[])
{
  /* NEED TO DO FOR LAB 7
  //ignore SIGPIPE so that we don't terminate when we call
  // send() on a disconnected socket.
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    DieWithError("signal() failed");
  */
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */

    // Test for correct number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <Database Filename> <Server Port>\n", argv[0]);
        exit(1);
    }
    // First arg is the database file name
    char *filename = argv[1];
    // Second arg is local port
    echoServPort = atoi(argv[2]);

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
                               &clntLen)) < 0)
            DieWithError("accept() failed");

        /* clntSock is connected to a client! */
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        HandleTCPClient(clntSock, filename);
    }
    /* NOT REACHED */
}
