#define _MN2214_ESC_
#define _MPU6050_

#include "../Superman/Drone.h"
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

    /*
    motor[0].calibration();
    motor[1].calibration();
    motor[2].calibration();
    motor[3].calibration();
    */

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
   yawPid.setTuning(kp, ki, kd);
    // 0.61 , 0.09, 0.16

    float roll, pitch, yaw;
    int pidRoll, pidPitch, pidYaw;

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

    int Emergency = 0;

    while(1)
    {
        flag=json_read(udp,&x,&y,&z,&t);

        /*
         * Input Action, roll, pitch, yaw, throttle
         */
        if(flag==1)
        {
            //   printf("control data: %.2f %.2f %.2f %d\n",x,y,z,t);

            throttle = t;
        }

        imu.getIMUData(&pitch, &roll, &yaw);

        if( pitch < -40 || pitch > 40 || roll < -40 || roll > 40)
        {
                Emergency = 1;
        }

        //yaw=0;
        //       json_write(udp,roll,pitch,yaw,0);

        pidRoll = rollPid.calcPID(x, roll);
        pidPitch = pitchPid.calcPID(y, pitch);
        pidYaw = yawPid.calcPID(z, yaw);

        Motor1_speed = throttle - pidRoll + pidPitch + pidYaw;
        Motor2_speed = throttle + pidRoll + pidPitch - pidYaw;
        Motor3_speed = throttle + pidRoll - pidPitch + pidYaw;
        Motor4_speed = throttle - pidRoll - pidPitch - pidYaw;

        if(Motor1_speed < 700) Motor1_speed = 700;
        if(Motor2_speed < 700) Motor2_speed = 700;
        if(Motor3_speed < 700) Motor3_speed = 700;
        if(Motor4_speed < 700) Motor4_speed = 700;


        if(cnt==200)
        {
            printf("ROLL : %3.6f, PITCH %3.7f, YAW %3.7f\n", roll, pitch, yaw);
            printf("pidRoll : %d, pidPitch : %d pidYaw: %d\n", pidRoll,pidPitch,pidYaw);
            printf("motor speed [1]:%d [2]:%d [3]:%d [4]:%d\n\n\n", Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed);
            cnt=0;
        }
            cnt++;

            if(Emergency)
            {
                Motor1_speed = 700;
                Motor2_speed = 700;
                Motor3_speed = 700;
                Motor4_speed = 700;
            }

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
            Emergency=0;
            printf("pid reset complete\n");
        }
        else if(tcp_flag==5)
        {
            printf("drone exit\n");
            break;
        }


    }//end while(1)


}//end int main()

