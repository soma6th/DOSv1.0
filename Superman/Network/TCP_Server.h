/*
 * TCP_Server.h
 *
 * @date 2015/10/01
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * TCP_Socket의 생성과 connect관련한 함수 선언
 */

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

// TCP client handling function
void HandleTCPClient(int clntSocket);

// Create TCP server socket
int CreateTCPServerSocket(unsigned short port);

// Accept TCP connection request
int AcceptTCPConnection(int servSock);

// return 1:1 connect socket discriptor
int TCP_connect_init(unsigned short port);
