#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"UDP_Server.h"

int main(int argc,char* argv[])
{
	int i;
	int clnt_sock;
	char buf[4];
	clnt_sock=Connected_UDP1(9190);
	for(i=0;i<10;i++)
	{
		write(clnt_sock,"123",strlen("123"));
	}
	for(i=0;i<10;i++)
	{
		read(clnt_sock,buf,strlen("123"));
	}

}
