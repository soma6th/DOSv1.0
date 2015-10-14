#include"Ctl.h"
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    int tcp_sock;
    
    tcp_sock=socket(PF_INET,SOCK_STREAM,0);
    
    printf("socket type one: %d \n",tcp_sock);
    
    change_mode(tcp_sock);
    
    sock_reuse(tcp_sock);
    
    return 0;
}
