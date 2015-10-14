#include"UDP_Server.h"
#include"TCP_Server.h"

#define _PORT_ 8003
#define MAXBUFF 100

int main(int argc,char* argv[])
{
	int tcp_sock,udp_sock;
	int str_len;
	char buf[MAXBUFF];
	struct sockaddr_in addr;
	int length;
	int i;

	memset(buf,0,sizeof(buf));

	strcpy(buf,"hello packet");
	tcp_sock=TCP_connect(_PORT_);
	recv(tcp_sock,buf,MAXBUFF-1,0);
	printf("r buf: %s\n",buf);
	send(tcp_sock,buf,MAXBUFF-1,0);
	printf("s buf: %s\n",buf);
	recv(tcp_sock,buf,MAXBUFF-1,0);
	printf("r buf: %s\n",buf);

	//udp_sock=CreateUDPServerSocket(8004);
	udp_sock=Connected_UDP(tcp_sock,8004);
	printf("tcp_sock: %d udp_sock: %d\n",tcp_sock,udp_sock);

	for(i=0;i<10;i++)
	{
		memset(buf,0,sizeof(buf));

		str_len=recv(udp_sock,buf,MAXBUFF-1,0);
		printf("recv: %s\n",buf);
		/*
		   if(str_len>0)
		   {
		   buf[str_len]=0;
		   str_len=recv(udp_sock,buf,MAXBUFF-1,MSG_PEEK|MSG_DONTWAIT);
		   printf("recv buf %s\n",buf);
		   }

		   else if(str_len==0)
		   break;
		 */
		
		str_len=send(udp_sock,buf,MAXBUFF,MSG_DONTROUTE|MSG_DONTWAIT);
		printf("send :%s\n",buf);
	//	sendto(udp_sock,buf,str_len,0,(struct sockaddr*)&addr,sizeof(length));
	}
	close(udp_sock);
	close(tcp_sock);

	return 0;
}
