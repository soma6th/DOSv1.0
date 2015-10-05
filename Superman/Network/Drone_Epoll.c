


int epoll_init()
{
    char message[BUF_SIZE];
    int serv_sock,clnt_sock;                   /* Socket descriptors for server */
    int i,max,str_len,recv_len,send_len;							//veriable for loop

    
    unsigned short portNo;           /* Actual port number */
    int running=1;
    
    //about socket descriptor variable
    int main_sock_fd;
    int main_sock_port;
    int port;                        /* Looping variable for ports */
    
    //time out setting variable
    long timeout;
    
    //about epoll variable
    struct epoll_event *ep_events;
    struct epoll_event event;
    
    int ep_fd,event_count;

    
    timeout = atol(argv[1]);        /* First arg: Timeout */
    port=atoi(argv[2]);				//listen socket port
    
    serv_sock=CreateTCPServerSocket(port);
    
    /*epoll setting*/
    //epoll 디스크립터가 생성될때 한번에 처리할 수 있는 socket의 크기 설정
    ep_fd=epoll_create(EPOLL_SIZE);
    //epoll이 담을 수 있는 구조체의 최대 크기를 설정
    ep_events=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    event.events=EPOLLIN;
    event.data.fd=serv_sock;
    //소켓을 추가하는 과정
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,serv_sock,&event);

    
    printf("Starting server:  Hit return to shutdown\n");
    while (running)
    {
        
        /*epoll setting flow*/
        
        if((event_count=epoll_wait(ep_fd,ep_events,EPOLL_SIZE,1000*timeout))==0)
        {
            //timeout flow
            continue;
        }
        
        
        for(i=0;i<event_count;i++)			//check get value
        {
            if(ep_events[i].data.fd==serv_sock)
            {
                //새로운 tcp 연결 요청이 들어왔을 경우
            }
            else if
            {
                
            }
        }//end for(i=0;i<max+1;i++)
        
    }	//end while(running)
    
    /*close server_sock (listen sock)*/
    close(serv_sock);
    close(ep_fd);
    
    return 0;
}