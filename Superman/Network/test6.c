#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"TCP_Server.h"


//MSG_PEEK and MSG_DONTWAIT test

int main(int argc,char* argv[])
{
    int sock;
    int i;
    int str_len;
    char buf[4];
    struct sockaddr_in send_adr;
    
    sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&send_adr,0,sizeof(send_adr));
    send_adr.sin_family=AF_INET;
    send_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    send_adr.sin_port=htons(atoi(argv[1]));
    
    if(connect(sock,(struct sockaddr*)&send_adr,sizeof(send_adr))==-1)
    {
        printf("connect error\n");
        close(sock);
        exit(1);
    }
    
    
    
    for(i=0;i<10;i++)
    {
        while(1)
        {
            memset(buf,0,sizeof(buf));
            str_len=recv(sock,buf,sizeof(buf)-1,MSG_PEEK|MSG_DONTWAIT);
            printf("strlen %d buf: %s \n",str_len,buf);
            if(str_len>0)
                break;
        }
        buf[str_len]=0;
        printf("again strlen %d buf: %s \n",str_len,buf);
    }
    
    printf("program exit\n");
    close(sock);
    return 0;
}
