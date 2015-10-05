




int network_init()
{
    int handling_fd,status_fd,control_fd;
    
    //socket create 부분
    /*handling을 위한 TCP socket 1개 생성과 controller에서 데이터를 전송받는것과 status를 받기 위한 UDP socket 2개*/
    hangling_fd=create_handling_socket();
    status_fd=create_status_socket();
    control_fd=create_control_socket();
    
    //socket 통신 방식, multi threading과, multiplexing 방식중 선택
    
    //만약 epoll이라면
    //epoll_setting
    epoll_init();
    
    //network loop
    epoll_mode();
    
}