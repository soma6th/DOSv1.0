#include"network.h"

int main(int argc,char* argv[])
{
    int tcp,udp,t;
    int flag,str_len;
    double x,y,z;
    char test_char,recv_flag;
    char pidbuf[100];
 
	json_t *data;
	json_error_t error;
    double pp,pi,pd;
    
    memset(pidbuf,0,sizeof(pidbuf));
    
	if(network_init(&tcp,&udp)==-1)
    {
        printf("socket init error\n");
        return 0;
    }

	printf("init complete network wait\n");
    
    for(;;)
	{
		json_write(udp,x,y,z,t);

		flag=json_read(udp,&x,&y,&z,&t);

		if(flag==1)
		{
			printf("control data:%.2f %.2f %.2f %d\n",x,y,z,t);
		}
		test_char=tcp_read(tcp);
		//pid reset
		if(test_char==3)
		{
			//str_len=recv(tcp,&test_char,1,MSG_DONTROUTE);
			printf("pid reset\n");
			//recv_len=0;
			//while(recv_len<str_len)
			//{
			//	recv_len+=recv(tcp,&pidbuf[recv_len],sizeof(pidbuf),MSG_DONTROUTE);
			//}
			str_len=recv(tcp,pidbuf,sizeof(pidbuf),MSG_DONTROUTE);
			pidbuf[str_len]=0;
			printf("recv pidbuf:%s\n",pidbuf);
			data=json_loads(pidbuf,JSON_DECODE_ANY,&error);
			if((json_unpack(data,"{s:f,s:f,s:f}","P_P",&pp,"P_I",&pi,"P_D",&pd))!=0)
			{
				//pid reset error
				printf("pid reset data recv error\n");
				recv_flag=0;
				send(tcp,&recv_flag,1,MSG_DONTROUTE);
				return 0;
			}
			recv_flag=1;
			//str_len=recv(tcp,pidbuf,sizeof(pidbuf),MSG_DONTROUTE);
			send(tcp,&recv_flag,1,MSG_DONTROUTE);
			printf("pid %.2f %.2f %.2f\n",pp,pi,pd);
		}
		else if(test_char==5)
		{
			printf("socket exit\n");
			break;
		}
	}

	return 0;
}
