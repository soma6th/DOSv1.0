/*
 * TCP_Server.c
 *
 * @date 2015/10/01
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * TCP_Socket의 생성과 해당 디스크립터 반환
 */

#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <string.h>     /* for memset() */
#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */
#include "Ctl.h"

#define MAXPENDING 100    /* Maximum outstanding connection requests */
#define RCVBUFSIZE 32   /* Size of receive buffer */

/*
 * @function : error message와 함께, 프로세스 종료
 * @param: errorMessage(해당하는 메세지를 터미널에 출력하기 위한 변수)
 * @return: void
 */
void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

/*
 * @function : ServerSocket을 생성하기 위한 함수 socket->bind->listen까지
 * @param: port(해당 포트번호로 Tcp server socket을 생성하기 위한 변수)
 * @return: 생성된 소켓의 디스크립터 -1일때는 socket 생성실패, 0일경우는
 * bind에러
 */
int CreateTCPServerSocket(unsigned short port)
{
    int sock;                        /* socket to create */
    struct sockaddr_in echoServAddr; /* Local address */
    
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    
    sock_reuse(sock);
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */
    
    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
    
    /* Mark the socket so it will listen for incoming connections */
    if (listen(sock, MAXPENDING) < 0)
        DieWithError("listen() failed");
    
    return sock;
}

/*
 * @function : CreateTCPServerSocket을 통해 생성된 Serversocket으로 accept하기 위한 함수
 * @param: servSock(ServerSocket의 디스크립터)
 * @return: 실제로 연결된 client와 1:1로 연결되어 있는 TCP소켓 디스크립터
 */
int AcceptTCPConnection(int servSock)
{
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure */
    
    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);
    
    /* Wait for a client to connect */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
                           &clntLen)) < 0)
        DieWithError("accept() failed");
    
    /* clntSock is connected to a client! */
    
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
    
    return clntSock;
}


/*
 * @function : CreateTCPServerSocket와 AccpetTCPConnection을 수행해, 실제 1:1
 * 로 client 연결된 소켓의 디스크립터만 가져오기 위한 함수, DOS-project에서는 더 이상의
 * TCP server socket이 필요없기때문에, close를 통해, 닫고, 1:1로 연결된 디스크립터만 반환한다.
 * @param: port(Server Socket을 열기위한 port number)
 * @return: 실제로 연결된 client와 1:1로 연결되어 있는 TCP소켓 디스크립터
 */
int TCP_connect(unsigned short port)
{
	int serv_sock;
	int clnt_sock;
	serv_sock=CreateTCPServerSocket(port);

	clnt_sock=AcceptTCPConnection(serv_sock);

	close(serv_sock);

	return clnt_sock;
}

