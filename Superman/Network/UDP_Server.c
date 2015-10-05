#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <string.h>     /* for memset() */
#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */


#define MAXPENDING 100    /* Maximum outstanding connection requests */
#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}


int CreateUDPServerSocket(unsigned short port)
{
    int sock;                        /* socket to create */
    struct sockaddr_in echoServAddr; /* Local address */
    
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        DieWithError("socket() failed");
    
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */
    
    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
    
    return sock;
}



void HandleUDPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    int client_len;                     /* client clientaddr size */
    
    struct sockaddr_in clientaddr;      /* send clientaddr */
    
    //UDP socket don't divide message, so 1 call recvfrom , 1 call sendto
    
    /* Receive message from client */
    if ((recvMsgSize = recvfrom(clntSocket, echoBuffer, RCVBUFSIZE, 0,
                                (struct sockaddr*)&clientaddr,&client_len)) < 0)
        DieWithError("recv() failed");
    

    /* Echo message back to client */
    if (sendto(clntSocket, echoBuffer, recvMsgSize, 0,
               (struct sockaddr*)&clientaddr,client_len) != recvMsgSize)
            DieWithError("send() failed");
    
    close(clntSocket);    /* Close client socket */
}

