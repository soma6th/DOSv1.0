#include"network.h"

int main(int argc,char* argv[])
{
    int tcp,udp,t;
    int flag;
    double x,y,z;
    char test_char;
    char pidbuf[100];
    
    double p,i,d;
    
    memset(pidbuf,0,sizeof(pidbuf));
    
	if(network_init(&tcp,&udp)==-1)
    {
        printf("socket init error\n");
        return 0;
    }

    x=10; y=20; z=30; t=40;
    
    for(;;)
	{
        test_char=tcp_read(tcp);
        //pid reset
        if(test_char==5)
        {
            recv(tcp,pidbuf,sizeof(pidbuf),MSG_DONTROUTE);
            if((json_unpack(data,"{s:f,s:f,s:f}","P_P",&p,"P_I",&i,"P_D",&d))!=0)
            {
                //pid reset error
                printf("json_unpack fail\n");
                test_char=0;
                send(tcp,&test_char,1,MSG_DONTROUTE);
                return 0;
            }
            test_char=1;
            recv(tcp,&test_char,1,MSG_DONTROUTE);
        }
        json_write(udp,x,y,z,t);
		flag=json_read(udp,&x,&y,&z,&t);
	}
	
	return 0;
}
