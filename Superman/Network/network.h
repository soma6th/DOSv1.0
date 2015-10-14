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
#include "UCP_Server.h"
#include "Ctl.h"
#include "Protocol.h"
#include "Drone_Epoll.h"

/*  UDP socket PORT number define */
#define __CONTROL_PORT__ 5556       //about controller
#define __STATUS_PORT__ 5554        //status data
#define __VIDEO_PORT__ 5555         //camara data not used

/*  TCP socket PORT number define */
#define __HANDLE_PORT__ 5559        //


#define __EPOLL__  0
#define __POLLING__  1

//socket create and setting
int network_init();

//network communication model setting and run
int network_run();

//happening abnomal event, connection weak or network exit also network exit
int network_exit();

//abnormal status handling function
int network_error();