/*
 * network.c
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * network의 선언을 간단하게 만들기 위한 함수.
 */


#include <stdio.h>      /* for printf() and fprintf() */
#include <unistd.h>
#include <jansson.h>
#include "network.h"

#define MAXBUFF 150

/*
 * @function :
 * @param:
 * waring:
 */
int network_init(int* tcp,int* udp)
{
    char buf;
    //create tcp socket
    *tcp=TCP_connect_init(__TCP_PORT__);
    if(*tcp<2)
    {
        printf("tcp init error\n");
        return -1;
    }
    
    *udp=UDP_connect_init(__UDP_PORT__);
    if(*udp<2)
    {
        printf("udp init error\n");
		return -1;
    }
    
    printf("init complete T:%d U:%d\n",*tcp,*udp);
	return 0;
}

/*
 * @function :
 * @param:
 * waring:
 */
int json_read(int socket,double* x,double* y,double* z,int* t)
{
    char buf[MAXBUFF];
    int header,str_len;
    json_t *data;
    json_error_t error;
    
    memset(buf,0,sizeof(buf));
    str_len=recv(socket,buf,MAXBUFF-1,MSG_DONTROUTE|MSG_DONTWAIT);
    if(str_len==-1)
	{
		return 0;
	}
	buf[str_len]=0;
	printf("control data: %s\n",buf);
    data=json_loads(buf,JSON_DECODE_ANY,&error);
    if((json_unpack(data,"{s:i,s:f,s:f,s:f,s:i}","P_H",&header,"P_X",x,"P_Y",y,"P_Z",z,"P_T",t))!=0)
    {
        //printf("json_unpack fail\n");
        return -1;
    }
	printf("data read success\n");
    return 1;

}

/*
 * @function :
 * @param:
 * waring:
 */
void json_write(int socket,double x,double y,double z,int t)
{
    char *buf;
    int header=2;
    json_t *data;
    
    data=json_pack("{s:i,s:f,s:f,s:f,s:i}","P_H",header,"P_X",x,"P_Y",y,"P_Z",z,"P_T",t);
    buf=json_dumps(data,JSON_ENCODE_ANY);
    send(socket,buf,strlen(buf),MSG_DONTROUTE|MSG_DONTWAIT);
}

/*
 * @function :
 * @param:
 * waring:
 */
int network_exit(int tcp,int udp)
{
    close(udp);
    close(tcp);
    printf("close all socket\n");
    return 0;
}

int tcp_read(int socket)
{
    char flag=0;
    int length;
    length=recv(socket,&flag,1,MSG_DONTROUTE|MSG_DONTWAIT);
    if(length==1)
    {
        if(flag)
        {
            printf("tcp data: %d\n",flag);
            return flag;
        }
    }
    return -1;
}

