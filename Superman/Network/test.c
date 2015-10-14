#include"TCP_Server.h"
#include"UDP_Server.h"
#include"Protocol.h"
#include"ctl.h"
#include<stdint.h>

#define MAXSTRING 20

int main(int argc,char* argv[])
{
	Packet packet;
	char buf[MAXSTRING];
		
	int tcp_sock;

	int udp_sock1;

	int read_size;

	int val;

	int i;

	int testint;

	struct sockaddr_in addd;
	int length;

	tcp_sock=TCP_connect((atoi(argv[1])));

	memset(buf,0,MAXSTRING);


	read_size=read(tcp_sock,buf,MAXSTRING);
	memcpy((void *)&packet,buf,sizeof(packet));

	printf("read buf: ");
	for(i=0;i<20;i++)
	{
		printf("%d ",buf[i]);
	}
	printf("\n");
	printf("packet %d %d %hd %.1f %.1f %.1f %d\n",packet.H,packet.F,packet.S,packet.X,packet.Y,packet.Z,ntohl(packet.T));
	

	packet.S=htons(9190);
	packet.X=12.5;
	packet.Y=ntohf1(12.5);
	packet.T=12;
	memcpy(buf,(void*)&packet,sizeof(packet));
	
	write(tcp_sock,buf,MAXSTRING);	
	printf("write buf: ");

	for(i=0;i<20;i++)
	{
		printf("%d ",buf[i]);
	}
	printf("\n");

	printf("packet %d %d %d %.1f %.1f %.1f %d\n",packet.H,packet.F,packet.S,packet.X,packet.Y,packet.Z,packet.T);

	memset(buf,0,MAXSTRING);
	read_size=read(tcp_sock,buf,MAXSTRING);

	printf("read buf: ");

	for(i=0;i<20;i++)
	{
		printf("%d ",buf[i]);
	}
	printf("\n");

	printf("packet %d %d %d %.1f %.1f %.1f %d\n",packet.H,packet.F,packet.S,packet.X,packet.Y,packet.Z,ntohl(packet.T));


	udp_sock1=CreateUDPServerSocket(htons(9190));


	memset(buf,0,MAXSTRING);
	//recvfrom(udp_sock1,buf,MAXSTRING,0,(struct sockaddr*)&addd,&length);
	read_size=read(udp_sock1,buf,MAXSTRING);

	printf("read buf: ");

	for(i=0;i<20;i++)
	{
		printf("%d ",buf[i]);
	}
	printf("\n");

	scanf("%d",&testint);
}
