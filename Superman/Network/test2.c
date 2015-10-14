#include"UDP_Server.h"
#include<stdio.h>

int main()
{
	int socket;
	short port=9999;

	socket=CreateUDPServerSocket(htons(port));

	scanf("%d",&port);
}
