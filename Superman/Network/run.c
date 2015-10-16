#include"network.h"

int main(int argc,char* argv[])
{
    int tcp,udp,t;
    int flag,i;
    double x,y,z;
    char test_char;
	if(network_init(&tcp,&udp)==-1)
    {
        printf("socket init error\n");
        return 0;
    }

    x=10; y=20; z=30; t=40;
    
	for(i=0;i<10;i++)
	{
        test_char=tcp_read(tcp);
        json_write(udp,x,y,z,t);
		flag=json_read(udp,&x,&y,&z,&t);
	}
	
	return 0;
}
