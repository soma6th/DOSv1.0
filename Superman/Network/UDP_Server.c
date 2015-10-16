/*
 * UCP_Server.c
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * UDP_Socket의 생성과 해당 디스크립터 반환
 */

#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <string.h>     /* for memset() */
#include <unistd.h>
#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */

#define MAXPENDING 100    /* Maximum outstanding connection requests */
#define UDPBUFSIZE 32   /* Size of receive buffer */


/*
 * @function : UDP socket을 생성하기 위한 함수 socket->bind까지
 * @param: port(해당 포트번호로 UDP server socket을 생성하기 위한 변수)
 * @return: 생성된 소켓의 디스크립터 -1일때는 socket 생성실패, 0일경우는
 * bind에러
 */
int CreateUDPServerSocket(unsigned short port)
{
    int sock;                        /* socket to create */
    struct sockaddr_in ServAddr; /* Local address */
	struct sockaddr_in taddr;
    char buf[150];
	int size;
	memset(buf,0,sizeof(buf));
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        return -1;
    }
    /* Construct local address structure */
    memset(&ServAddr, 0, sizeof(ServAddr));   /* Zero out structure */
    ServAddr.sin_family = AF_INET;                /* Internet address family */
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    ServAddr.sin_port = htons(port);              /* Local port */
    
    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
    {
        return 0;
    }

	printf("Create socket: %s:%d\n",inet_ntoa(ServAddr.sin_addr),ntohs(ServAddr.sin_port));
    
    return sock;

}

/*
 * @function : 만약에 1:1로 계속 왔다갔다하는 UDP socket을 생성하게 된다면,
 * 그냥 socket을 생성하는것보다, connected udp를 열게 되면, UDP socket에
 * IP와 PORT번호 정보가 등록되어, 성능향상을 기대 할 수 있다. 또한 기존의 UDP함수가
 * recvfrom이나, sendto를 사용했던것에 반해 read,write함수를 사용 가능하므로,
 * 실제 코드의 효율성도 증가함으로, UDP는 생성하자마자 connected udp를 지향한다.
 *
 * @param: tcp(getpeername으로 상대방의 ip를 확인하기 위한 변수)
 * port(상대 udp port번호를 알기 위한 변수)
 * @return: 실제로 연결된 client와 1:1로 연결되어 있는 TCP소켓 디스크립터
 */
int UDP_connect_init(unsigned short port)
{
    int udp;
    
    struct sockaddr_in udp_addr;
	struct sockaddr_in con_addr;
    
	char buf[100];
	socklen_t length;


	memset(buf,0,sizeof(buf));
	if((udp=socket(PF_INET,SOCK_DGRAM,0))<0)
		return -1;

	//setting for 0
	memset(&udp_addr,0,sizeof(udp_addr));
	udp_addr.sin_family=AF_INET;
	//inet_aton("10.10.0.1",&udp_addr.sin_addr);
	udp_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	udp_addr.sin_port=htons(port);

	//Bind to the local address
	if (bind(udp,(struct sockaddr *) &udp_addr, sizeof(udp_addr)) < 0)
		return 0;

	printf("server ip is %s\n",inet_ntoa(udp_addr.sin_addr));
	printf("server port is %d %d\n",udp_addr.sin_port,ntohs(udp_addr.sin_port));

	memset(&con_addr,0,sizeof(con_addr));

	//important if can't call, can't read client address
	length=sizeof(con_addr);

	recvfrom(udp,buf,100-1,0,(struct sockaddr*)&con_addr,&length);

	printf("UDP_init recvfrom buf: %s\n",buf);

	if(connect(udp,(struct sockaddr*)&con_addr,sizeof(con_addr))<0)
	{
		printf("udp connect error\n");
		exit(1);
	}
    printf("connected ip:port %s:%d\n",inet_ntoa(con_addr.sin_addr),ntohs(con_addr.sin_port));
    
    send(udp,buf,sizeof(buf),0);
    recv(udp,buf,sizeof(buf),0);
    
    printf("UDP_init recv buf: %s\n",buf);
    
	return udp;
}
