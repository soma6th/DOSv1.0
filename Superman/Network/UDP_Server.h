/*
 * UDP_Server.h
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * TCP_Socket의 생성과 셋팅에 관한 함수 선언
 */


#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

/// Create UDP server socket
int CreateUDPServerSocket(unsigned short port);
/// connected UDP socket setting
int Connected_UDP(unsigned short port);