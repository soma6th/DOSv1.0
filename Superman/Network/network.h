/*
 * Drone_Epoll.h
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * 패킷을 받는 방법에 대한 방법론중, epoll에 관련된 함수의 선언
 */

#include "TCP_Server.h"
#include "UDP_Server.h"
#include <jansson.h>
//#include "Ctl.h"
//#include "Protocol.h"

/*  UDP socket PORT number define */
#define __UDP_PORT__ 8004
/*  TCP socket PORT number define */
#define __TCP_PORT__ 8003        //

//#define __EPOLL__  0
//#define __POLLING__  1

//socket create and setting
int network_init(int* tcp,int* udp);

int json_read(int socket,double* x,double* y,double* z,int* t);

void json_write(int socket,double x,double y,double z,int t);

int tcp_read(int socket);

//happening abnomal event, connection weak or network exit also network exit
int network_exit(int tcp,int udp);

//abnormal status handling function
int network_error();
