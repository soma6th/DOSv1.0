#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"TCP_Server.h"

int main(int argc,char* argv[])
{
	int serv_sock;
	int clnt_sock;
	int i;
	
	clnt_sock=TCP_connect(atoi(argv[1]));

	for(i=0;i<10;i++)
	{
		write(clnt_sock,"123",strlen("123"));
	}
	close(clnt_sock);
}
