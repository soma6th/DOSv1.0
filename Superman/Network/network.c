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
#define MAXBUFF 150;

/*
 * @function :
 * @param:
 * waring:
 */
int network_init()
{
    char buf[MAXBUFF];
    int tcp;
    int udp;
    //create tcp socket
    tcp=TCP_connect(__TCP_PORT__);
    memset(buf,0,sizeof(buf));
    recv(tcp,buf,MAXBUFF-1,0);
    printf("r buf: %s\n",buf);
    send(tcp,buf,MAXBUFF-1,0);
    printf("s buf: %s\n",buf);
    recv(tcp,buf,MAXBUFF-1,0);
    printf("r buf: %s\n",buf);
    
    udp=Connected_UDP(__UDP_PORT__);
    if(udp_sock<2)
    {
        printf("socket create error\n");
        exit(1);
    }
    printf("tcp_sock: %d udp_sock: %d\n",tcp,udp);
    
    return udp;
}

/*
 * @function :
 * @param:
 * waring:
 */
int network_read(int socket,float* x,float* y,float* z,int* t)
{
    char buf[MAXBUFF];
    int header;
    json_t *data;
    json_error_t error;
    
    memset(buf,0,sizeof(buf));
    str_len=recv(socket,buf,MAXBUFF-1,MSG_DONTROUTE|MSG_DONTWAIT);
    buf[str_len]=0;
    
    test=json_loads(buf,JSON_DECODE_ANY,&error);
    
    if((json_unpack(root,"{s:i,s:f,s:f,s:f,s:i}","P_H",&header,"P_X",x,"P_Y",y,"P_Z",z,"P_T",t))!=0)
    {
        printF("json_unpack fail\n");
        return 0;
    }
    
    return -1;
}

/*
 * @function :
 * @param:
 * waring:
 */
void network_write(int socket,float x,float y,float z,int t)
{
    char buf[MAXBUFF];
    int header=2;
    json_t *data;
    
    data = json_pack("{s:i,s:f,s:f,s:f,s:i}","P_H",header,"P_X",x,"P_Y",y,"P_Z",z,"P_T",t);
    buf = json_dumps(data, JSON_ENCODE_ANY);
    send(socket,buf,MAXBUFF-1,MSG_DONTROUTE|MSG_DONTWAIT);
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



