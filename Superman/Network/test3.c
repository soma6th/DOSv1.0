#include"Protocol.h"

int main()
{
    Packet packet;
    
    
    char buf[5];
    char test[20];
    buf[0]=1;
    buf[1]=2;
    buf[2]=3;
    buf[3]=4;
    
    packet.H=10;
    packet.X=15;
    packet.F=5;
    packet.T=20;
    
    test=struct_to_string(packet);
    string_to_struct(buf);
    
    printf("%d %d\n",test[0],test[1]);
	printf("\n");
}
