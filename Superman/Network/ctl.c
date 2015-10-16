/*
 * Ctl.c
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * socket의 설정과 확인에 대한 함수 구현.
 */

#include<assert.h>
#include<string.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>

void get_string_access_mode2(int val,char * ret)
{
	switch(val & O_ACCMODE)
	{
		case O_RDONLY: strcpy(ret,"read only"); break;
		case O_WRONLY: strcpy(ret,"write only"); break;
		case O_RDWR: strcpy(ret,"both read write"); break;
		default: strcpy(ret,"unknown access mode");
	}

	if(val & O_APPEND)
		sprintf(ret,"%s, append",ret);
	if(val & O_NONBLOCK)
		sprintf(ret,"%s, nonblocking",ret);
	if(val & O_SYNC)
		sprintf(ret,"%s, synchronous",ret);
	if(val & O_ASYNC)
		sprintf(ret,"%s, asynchronous",ret);

	ret = strcat(ret,"\n");

}


int async_test(int fd,int flag)
{
	int val;

	char confirm[100];

	if((val=fcntl(fd,F_GETFL,0))<0)
		assert("get fcntl");

	get_string_access_mode2(val,confirm);

	printf("before file control : %s \n",confirm);

	//add setting
	//val |= O_APPEND | O_DSYNC;
	
	get_string_access_mode2(val,confirm);

	printf("after file contorl : %s \n",confirm);

}

int change_mode(int socket)
{
    int state;
    int sock_type;
    socklen_t optlen;
    
    optlen=sizeof(sock_type);
    
    
    if(getsockopt(socket,SOL_SOCKET,SO_TYPE,(void *)&sock_type,&optlen)==-1)
    {
        printf("socket option get fail\n");
        return -1;
    }
    
    printf("socket type one: %d \n",sock_type);
    return 0;
}

int sock_reuse(int socket)
{
    int option=1;
	socklen_t optlen;
    
    if(setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(void*)&option,optlen)==-1)
    {
        printf("sock_reuse error");
        exit(1);
    }
    
    return 0;
}




