/*
 * Protocol.c
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * 각 socket의 flag에 따른, socket의 절차와,
 * 데이터 연산에 대한 함수 구현
 */

#include"Protocol.h"

// struct's size
#define STRUCT_SIZE 20

//char* buf : const char array
//char buf[] : char array

/*
 * @function : char array에 있는 데이터를 packet구조체에 넣는 함수
 * @param: packet(buf에 집어넣을 데이터가 있는 struct) buf(struct의 데이터가 들어가는 char)
 * waring: char를 리턴뒤에도 사용해야하기 때문에, argu에 집어넣는다.
 */
Packet string_to_struct(char* buf)
{
    Packet packet;
    memcpy(&packet,buf,STRUCT_SIZE);
	return packet;
}

/*
 * @function : packet구조체의 데이터를 char array로 넘기는 함수
 * @param: packet(buf에 집어넣을 데이터가 있는 struct) buf(struct의 데이터가 들어가는 char)
 * waring: char를 리턴뒤에도 사용해야하기 때문에, argu에 집어넣는다.
 */
void struct_to_string(Packet packet.char *buf)
{
    memcpy(buf,&packet,STRUCT_SIZE);
    return buf;
}


/*
 * @function : float형 변수는, htonl함수가 안먹는 경우가 있으므로, 바꿔주는 함수이다.
 * @param: f(byteorder)를 바꾸고 싶은 float
 * @return: host byte order로 바뀐 float 변수
 */
float ntohf(float f)
{
	float r;
	unsigned char *s = (unsigned char*)&f;
	unsigned char *d = (unsigned char*)&r;
	d[0]=s[3];
	d[1]=s[2];
	d[2]=s[1];
	d[3]=s[0];
	return r;
}

/*
 * @function : float형 변수는, htonl함수가 안먹는 경우가 있으므로, 바꿔주는 함수이다.
 * @param: f(byteorder)를 바꾸고 싶은 float
 * @return: network byte order로 바뀐 float 변수
 */
float htonf(float f)
{
    float r;
    unsigned char *s = (unsigned char*)&f;
    unsigned char *d = (unsigned char*)&r;
    d[0]=s[3];
    d[1]=s[2];
    d[2]=s[1];
    d[3]=s[0];
    return r;
}

/*
 * @function : network byte order인지 host byte order인지 확인하는 함수
 * @return: network byte order면 1 아니면 0
 */
int endian_check()
{
    int i = 0x00000001;
    
    //little endian is not network byte order
    if( ((char *)&i)[0] )
        printf( "Littile Endian\n" );
    else
        printf( "Big Endian\n" );
    
    return 1;
}


/*
 * @function : char array의 buffer를 출력하기 위한 버퍼
 * @param : buf(값을 출력하기 위한, 변수)
 */
void print_buf(char* buf)
{
    int i;
    for(i=0;i<20;i++)
    {
        printf("%d ",buf[i]);
    }
    printf("\n");
}

/*
 * @function : packet의
 * @param: packet(값을 출력하기 위한 변수)
 */
void print_packet(Packet packet)
{
    printf("%d %d %d %.2f %.2f %.2f %d\n",
           packet.H,packet.F,packet.S,packet.X,packet.Y,packet.Z,packet.T);
}


void packet_swticher(Packet packet)
{
    switch (packet.F) {
        case 0: init_message(); break;
        case 1:  break;
        case 2: recv_control(); break;
        case 3: send_status(); break;
        default: break;
    }
}

/*
 * @function : init 절차를 수행하는 프로세스 flag==1일때,
 * @return: 미정
 */
// init, when TCP flag == 0
int init_message(int socket)
{
    Packet packet;
    char buf[STRUCT_SIZE];
    int read_size;
    
    memset(&packet,0,STRUCT_SIZE);
    packet.H=0xff;
    packet.S=1;
    
    memcpy(buf,&packet,STRUCT_SIZE);
    
    write(socket,buf,STRUCT_SIZE);
    
    printf("write buf: ");
    
    print_buf(buf);
    
    printf("write packet: ");
    
    print_packet(packet);
    
    read_size=read(socket,buf,MAXSTRING);
    
    printf("read buf: ");
    
    print_buf(buf);
    
    printf("read packet: ");
    
    print_packet(packet);
    
}



/*
 * @function : control message를 받는 패킷
 * @return: 미정
 */
// receive control packet, when TCP flag == 2
void recv_control(int socket)
{
    
}

/*
 * @function : status message를 보내는 패킷
 * @return: 미정
 */
// receive control packet, when TCP flag == 3
void send_status(int socket)
{
    
}




