#include"network.h"

int main(int argc,char* argv[])
{
	int socket;
	int flag,i;
	float x,y,z;
	int t;

	socket=network_init();

	for(i=0;i<10;i++)
	{
		flag=network_read(socket,&x,&y,&z,&t);
	}
	
	return 0;
}
