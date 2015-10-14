/*
 * Protocol.h
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * Packet의 struct구조와, 데이터연산, flow에 대한 함수 선언
 */

#include<stdio.h>  /* for perror() */
#include<stdlib.h> /* for exit() */
#include<string.h> /* for memset() */
#include<unistd.h>
#include<stdint.h>

/**
 @struct  Packet
 @date    2015/10/08
 @author  이제우(wpdn006@gmail.com)
 @brief   Packet의 구조
 @warning
 */
typedef struct _PACKET_{
	uint8_t	H; ///< Header 부분이다. 0xEE로 고정
	uint8_t F; ///< flag부분이다. 이 flag를 통한 switch문으로 함수를 실행한다.
	uint16_t S; ///< sequence 부분이다. 추후에 결정할 예정이다.
	float X; ///< controll시에는 x축에 대한 데이터를 보낸다.
	float Y; ///< controll시에는 y축에 대한 데이터를 보낸다.
	float Z; ///< controll시에는 z축에 대한 데이터를 보낸다.
	uint32_t T; ///< controll시에는 thoughput에 대한 데이터를 보낸다.
    
    // 다른 경우에는 X,Y,Z,T는 데이터를 실어보내는 역할을 한다.//
}Packet;

// init, when TCP flag == 0
int init_message(int socket,int version,int flag);

int convert_message(char* buf);

// network to host byte order for float
float ntohf(float f);

// host to network byte order for float
float htonf(float f);

// enddian check function
int endian_check();

// buf->Packet(struct)
Packet string_to_struct(char* buf);

// Packet(struct)->buf
void struct_to_string(Packet packet.char *buf);

// print char array
void print_buf(char* buf);

// print struct
void print_packet(Packet packet);

// controll packet receive function
void recv_control(int socket);

// status packet send function
void send_status(int socket);