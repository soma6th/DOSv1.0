
#include"TCP_Server.h"

#define _PORT_ 8003
#define MAXBUFF 100

int main(int argc,char* argv[])
{
	int tcp_sock;
	int str_len;
	char buf[MAXBUFF];

	memset(buf,0,sizeof(buf));

	tcp_sock=TCP_connect(_PORT_);

	while(1)
	{
		memset(buf,0,sizeof(buf));
		str_len=recv(tcp_sock,buf,MAXBUFF-1,MSG_PEEK|MSG_DONTWAIT);
		if(str_len>0)
		{
			buf[str_len]=0;
			str_len=recv(tcp_sock,buf,MAXBUFF-1,MSG_PEEK|MSG_DONTWAIT);
			printf("recv buf %s\n",buf);
		}
		else if(str_len==0)
			break;

		str_len=send(tcp_sock,buf,MAXBUFF,MSG_DONTROUTE|MSG_DONTWAIT);
		printf("send buf %s\n",buf);
	}
	close(tcp_sock);

	return 0;
}
