
#define _MN2214_ESC_
#define _MPU6050_

#include "../Superman/Drone.h"
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

using namespace std;

void sig_int(int signo)
{
    digitalWrite(22, LOW);
    digitalWrite(23, LOW);
    digitalWrite(24, LOW);
    digitalWrite(25, LOW);

    exit(0);
}

int main(int argc, char* argv[])
{
    // Socket
    int throttle[4]={1000,1000,1000,1000};
    int socket;
    float n_roll, n_pitch, n_yaw;
    int n_dc;


    //    socket = network_init();


    void sig_int(int signo);

    struct sigaction intsig;
    intsig.sa_handler = sig_int;
    sigemptyset(&intsig.sa_mask);
    intsig.sa_flags = 0;
    sigaction(SIGINT, &intsig, 0);

    Motor motor[4];
    IMU imu;

    for(int i = 0 ; i < 4 ; i++)
    {
        motor[i].init();
    }

    motor[0].setPin(22);
    motor[1].setPin(23);
    motor[2].setPin(24);
    motor[3].setPin(25);

    if(imu.init())
    {
        printf("ERROR IMU!!\n");
        return 0; //센서 연결 실패
    }
    // Non block input char
    struct termios oldSettings, newSettings;

    tcgetattr(fileno(stdin), &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &newSettings);

    unsigned char ch = '0';


    int Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed;
    printf("Start!!!\n");
    while(1)
    {

        //        if(network_read(socket, &n_roll, &n_pitch, &n_yaw, &n_dc))
        //        {
        //            throttle = n_dc;
        //        }

        // NON Block
        fd_set set;
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 100;

        FD_ZERO(&set);
        FD_SET(fileno(stdin), &set);

        int res = select(fileno(stdin)+1, &set, NULL, NULL, &tv);

        if(res > 0)
        {
            read(fileno(stdin), &ch, 1);

            switch(ch)
            {
                case 'q':

                    motor[0].setSpeed(0);
                    motor[1].setSpeed(0);
                    motor[2].setSpeed(0);
                    motor[3].setSpeed(0);
                    tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
                    return 0;
                    break;
                case 'r':

                    throttle[0] = 1000;
                    throttle[1] = 1000;
                    throttle[2] = 1000;
                    throttle[3] = 1000;
                    break;

                case 'a':
                    throttle[0]++;
                    break;

                case 's':
                    throttle[1]++;
                    break;

                case 'd':
                    throttle[2]++;
                    break;

                case 'f':
                    throttle[3]++;
                    break;
                case 'z':
                    throttle[0]--;
                    break;
                case 'x':
                    throttle[1]--;
                    break;
                case 'c':
                    throttle[2]--;
                    break;
                case 'v':
                    throttle[3]--;
                    break;
        printf("t[0]: %d t[1]: %d t[2]: %d t[3]: %d\n",throttle[0],throttle[1],throttle[2],throttle[3]);
            }

        }
        else if(res < 0)
        {
            printf("res < 0!!\n");
            break;
        }
        else
        {

        }

        printf("t[0]: %d t[1]: %d t[2]: %d t[3]: %d\n",throttle[0],throttle[1],throttle[2],throttle[3]);
        motor[0].setSpeed(throttle[0]); 
        motor[1].setSpeed(throttle[1]);
        motor[2].setSpeed(throttle[2]);
        motor[3].setSpeed(throttle[3]);       
    }



}
