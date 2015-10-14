/*
 * Drone_Epoll.h
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * 패킷을 받는 방법에 대한 방법론중, epoll에 관련된 함수의 선언
 */

#include "TCP_Server.h"  /* TCP echo server includes */
#include <sys/time.h>       /* for struct timeval {} */
#include <sys/wait.h>		//wait pid function
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>          /* for fcntl() */
#include <unistd.h>
#include <signal.h>			//signal handling



//
int epoll_init();

int epoll_run(int tcp1,int udp1,int udp2,int timeout);
