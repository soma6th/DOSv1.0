#define _EMAX_30A_ESC_

#define _MPU6050_

#include "Sensor/Sensor.h"
#include "Motor/Motor.h"
#include "../Network/network.h"
#include "PID/PID.h"
#include <stdio.h>
#include <signal.h>
#include <time.h>

// 1.2, 0.00084, 0.61340

int main(int argc, char* argv[])
{
    int cnt=0;
    //network about
    int tcp,udp;
    int flag,str_len,t;
    double x,y,z;
    double pp,pi,pd;
    char pidbuf[100],tcp_flag,recv_flag;
    json_t *data;
    json_error_t error;
    

    int throttle = 0;

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

    motor[0].calibration();
    motor[1].calibration();
    motor[2].calibration();
    motor[3].calibration();


    if(imu.init())
    {
        printf("Plese check the MPU6050 device connection!!\n");
        return 0; //센서 연결 실패
    }

    PID rollPid, pitchPid, yawPid;

    rollPid.init();
    pitchPid.init();
    yawPid.init();

    float kp, ki, kd;

    kp = 0.f; // 1.2
    ki = 0.f; // 0.00084
    kd = 0.f; // 0.6134

    rollPid.setTuning(kp, ki, kd);
    pitchPid.setTuning(kp, ki, kd);
   // 0.61 , 0.09, 0.16

    float roll, pitch, yaw=0;
    int pidRoll, pidPitch, pidYaw=0;

    imu.calibration();



    int Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed;
     

    printf("Connect Application!!\n");

    //network init
    if(network_init(&tcp,&udp)==-1)
    {
        printf("socket create error\n");
        return 0;
    }
    printf("network init complete\n");


    // 
    int tmpR = 0;
    int tmpP = 0;
    int tmpY = 0;


    while(1)
    {
        flag=json_read(udp,&x,&y,&z,&t);

        /*
         * Input Action, roll, pitch, yaw, throttle
         */
        if(flag==1)
        {
            //   printf("control data: %.2f %.2f %.2f %d\n",x,y,z,t);

            throttle = t/10;
        }

        imu.getIMUData(&pitch, &roll, &yaw);

        yaw=0;
        //       json_write(udp,roll,pitch,yaw,0);

        pidRoll = rollPid.calcPID(0, roll);
        pidPitch = pitchPid.calcPID(0, pitch);
        pidYaw = yawPid.calcPID(0, yaw);

        Motor1_speed = throttle - pidRoll + pidPitch - pidYaw;
        Motor2_speed = throttle + pidRoll + pidPitch + pidYaw;
        Motor3_speed = throttle + pidRoll - pidPitch - pidYaw;
        Motor4_speed = throttle - pidRoll - pidPitch + pidYaw;

        if(Motor1_speed < 0) Motor1_speed = 0;
        else if(Motor1_speed >240) Motor1_speed = 240;
        if(Motor2_speed < 0) Motor2_speed = 0;
        else if(Motor2_speed >240) Motor2_speed = 240;
        if(Motor3_speed < 0) Motor3_speed = 0;
        else if(Motor3_speed >240) Motor3_speed = 240;
        if(Motor4_speed < 0) Motor4_speed = 0;
        else if(Motor4_speed >240) Motor4_speed = 240;


        //if(pidRoll != tmpR || pidPitch != tmpP)
        if(cnt==200)
        {
            printf("ROLL : %3.6f, PITCH %3.7f, YAW %3.7f\n", roll, pitch, yaw);
            printf("pidRoll : %d, pidPitch : %d pidYaw: %d\n", pidRoll,pidPitch,pidYaw);
            printf("motor speed [1]:%d [2]:%d [3]:%d [4]:%d\n\n\n", Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed);
            cnt=0;
        }
        else if(pidRoll != tmpR || pidPitch != tmpP)
        {
            cnt++;
        }

        json_write(udp,roll,pitch,yaw,0);

        tmpR = pidRoll;
        tmpP = pidPitch;
        tmpY = pidYaw;

        motor[0].setSpeed(Motor1_speed);
        motor[1].setSpeed(Motor2_speed);
        motor[2].setSpeed(Motor3_speed);
        motor[3].setSpeed(Motor4_speed);


        tcp_flag=tcp_read(tcp);
        if(tcp_flag==3)
        {
            memset(pidbuf,0,sizeof(pidbuf));
            printf("pid reset flag\n");
            str_len=recv(tcp,pidbuf,sizeof(pidbuf),MSG_DONTROUTE);
            pidbuf[str_len]=0;
            printf("pid reset data :%s\n",pidbuf);
            data=json_loads(pidbuf,JSON_DECODE_ANY,&error);
            if((json_unpack(data,"{s:f,s:f,s:f}","P_P",&pp,"P_I",&pi,"P_D",&pd))!=0)
            {
                printf("piid reset error\n");
                recv_flag=0;
                send(tcp,&recv_flag,1,MSG_DONTROUTE);
                return 0;
            }
            recv_flag=1;

            send(tcp,&recv_flag,1,MSG_DONTROUTE);

            rollPid.initKpid(pp, pi, pd);
            pitchPid.initKpid(pp, pi, pd);
            yawPid.initKpid(pp, pi, pd);

            printf("pid reset complete\n");
        }
        else if(tcp_flag==5)
        {
            printf("drone exit\n");
            break;
        }


    }//end while(1)


}//end int main()

