/*
 * Ctl.h
 *
 * @date 2015/10/08
 * @author Leejewoo
 * @email nowwoo91@gmail.com
 *
 * socket의 설정과 확인에 대한 함수 선언.
 */



void get_string_access_mode(int val,char* ret);

int async_test(int fd,int flag);

int change_mode(int socket);

int sock_reuse(int socket);
