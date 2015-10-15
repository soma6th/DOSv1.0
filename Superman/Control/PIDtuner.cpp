#define __NOESC__
#define _MPU6050_

#include "Sensor/Sensor.h"
#include "Motor/Motor.h"
#include "PID/PID.h"
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "../Network/network.h"

using namespace std;

void sig_int(int signo)
{
    digitalWrite(22, LOW);
    digitalWrite(23, LOW);
    digitalWrite(24, LOW);
    digitalWrite(25, LOW);

    exit(0);
}

int main()
{
    // Socket
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

    int throttle = 1000;

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

    PID rollPid, pitchPid, yawPid;

    rollPid.init();
    pitchPid.init();
    //yawPid.init();

    float kp, ki, kd;

    kp = 0.71; // 0.71
    ki = 0.09; // 0.09
    kd = 0.16; // 0.16

    rollPid.setTuning(kp, ki, kd);
    pitchPid.setTuning(kp, ki, kd);
   // 0.61 , 0.09, 0.16
    //yawPid.setTuning(0.168, 0.0654, 0.008);

    float roll, pitch, yaw;
    int pidRoll, pidPitch, pidYaw;

    imu.calibration();

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

                case 'a':
                    kp += 0.01;
                    break;

                case 's':
                    ki += 0.01;
                    break;

                case 'd':
                    kd += 0.01;
                    break;

                case 'z':
                    kp -= 0.01;
                    break;

                case 'x':
                    ki -= 0.01;
                    break;

                case 'c':
                    kd -= 0.01;
                    break;
                case 'f':
                    throttle += 1;
                    break;

                case 'v':
                    throttle -= 1;
                    break;
            
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

       printf("P : %.02f, I : %.02f, D : %.02f\n", kp, ki, kd);

       
       imu.getIMUData(&roll, &pitch, &yaw);
       pidRoll = rollPid.calcPID(0, roll);
       pidPitch = pitchPid.calcPID(0, pitch);
        //pidYaw = yawPid.calcPID(0, yaw);

          Motor1_speed = throttle - pidPitch*40; //40
          Motor2_speed = throttle + pidRoll*40;
          Motor3_speed = throttle - pidRoll*40;
          Motor4_speed = throttle - pidPitch*40;
        
//          motor[0].setSpeed(Motor1_speed); 
          motor[1].setSpeed(Motor2_speed);
//          motor[2].setSpeed(Motor3_speed);
         motor[3].setSpeed(Motor4_speed);       
    
/*
        
        printf("input : %c\n", ch);

   printf("P : %.02f, I : %.02f, D : %.02f\n", kp, ki, kd);


//        printf("[r = %f] [p = %f]  [PID = %f %f %f] MOTOR 1 : %d  MOTOR 2 : %d  MOTOR3 : %d  MOTOR4 : %d\n",
//                roll, pitch, kp, ki, kd, Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed);

        }

        */
    }



}
