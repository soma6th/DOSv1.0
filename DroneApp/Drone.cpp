#define _MN2214_ESC_
#define _MPU6050_

#include "../Superman/Drone.h"
#include <stdio.h>
#include <signal.h>
#include <time.h>

// 1.2, 0.00084, 0.61340

int main(int argc, char* argv[])
{
    //yaw pid
    int count;
    char ff;

    int cnt=0;
    //network about
    int tcp,udp;
    int flag,str_len,t=0;
    double x = 0,y=0,z=0;
    double pp,pi,pd;
    char pidbuf[100],tcp_flag,recv_flag;
    json_t *data;
    json_error_t error;

    double yp,yi,yd;
    double kp, ki, kd;
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


    kp = 2.0; // 1.2
    ki = 0.1; // 0.00084
    kd = 0.85; // 0.6134

    rollPid.setTuning(0.f, 0.f, 0.f);
    pitchPid.setTuning(0.f, 0.f, 0.f);
    yawPid.setTuning(0.f, 0.f, 0.f);
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

        imu.getIMUData(&roll, &pitch, &yaw);

        if( pitch < -40 || pitch > 40 || roll < -40 || roll > 40)
        {
            Emergency = 1;
        }

        //yaw=0;
        //       json_write(udp,roll,pitch,yaw,0);

        pidRoll = rollPid.calcPID(x*(2), roll);
        pidPitch = pitchPid.calcPID(y*(-2), pitch);
        pidYaw = yawPid.calcPID(z*(2), yaw);

//        Motor1_speed = throttle + pidPitch + pidYaw;
//        Motor2_speed = throttle - pidRoll - pidYaw;
//        Motor3_speed = throttle - pidPitch + pidYaw;
//        Motor4_speed = throttle + pidRoll - pidYaw;

        Motor1_speed = throttle + pidRoll + pidPitch + pidYaw;
        Motor2_speed = throttle - pidRoll + pidPitch - pidYaw;
        Motor3_speed = throttle - pidRoll - pidPitch + pidYaw;
        Motor4_speed = throttle + pidRoll - pidPitch - pidYaw;

        if(cnt==100)
        {
            printf("X: %.2lf Y: %.2lf Z: %.2lf T:%d \n",x,y,z,t);
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

        if(Motor1_speed < 700) Motor1_speed = 700;
        if(Motor2_speed < 700) Motor2_speed = 700;
        if(Motor3_speed < 700) Motor3_speed = 700;
        if(Motor4_speed < 700) Motor4_speed = 700;

        motor[0].setSpeed(Motor1_speed);
        motor[1].setSpeed(Motor2_speed);
        motor[2].setSpeed(Motor3_speed);
        motor[3].setSpeed(Motor4_speed);

        tcp_flag=tcp_read(tcp);
        if(tcp_flag==3)
        {
            motor[0].setSpeed(700);
            motor[1].setSpeed(700);
            motor[2].setSpeed(700);
            motor[3].setSpeed(700);

            memset(pidbuf,0,sizeof(pidbuf));
            printf("pid reset flag\n");
            str_len=recv(tcp,pidbuf,sizeof(pidbuf),MSG_DONTROUTE);
            pidbuf[str_len]=0;
            printf("pid reset data :%s\n",pidbuf);
            data=json_loads(pidbuf,JSON_DECODE_ANY,&error);
            if((json_unpack(data,"{s:f,s:f,s:f}","P_P",&pp,"P_I",&pi,"P_D",&pd))!=0)
            {
                printf("pid reset error\n");
                recv_flag=0;
                send(tcp,&recv_flag,1,MSG_DONTROUTE);
                return 0;
            }
            recv_flag=1;

            send(tcp,&recv_flag,1,MSG_DONTROUTE);
               
            printf("input Yaw's pid : ");
            scanf("%lf %lf %lf",&yp,&yi,&yd);
            rollPid.initKpid(pp, pi, pd);
            pitchPid.initKpid(pp, pi, pd);
            yawPid.initKpid(yp, yi, yd);
            Emergency=0;
           
            while(1)
            {

                str_len=recv(tcp,&ff,1,MSG_DONTROUTE|MSG_DONTWAIT);
                str_len=recv(tcp,pidbuf,sizeof(pidbuf),MSG_DONTROUTE|MSG_DONTWAIT);
                recv_flag=1;
                send(tcp,&recv_flag,1,MSG_DONTROUTE);
                if(str_len==-1)
                {
                    printf("nomore data dump\n");
                    break;
                }


                //printf("roll and pitch kp: %.2lf ki: %.2lf kd: %.2lf\n",kp,ki,kd);
                //printf("yaw pp: %.2lf pi: %.2lf pd: %.2lf\n",pp,pi,pd);
                printf("pid reset complete\n");
            }
        }
        else if(tcp_flag==5)
        {

            motor[0].setSpeed(600);
            motor[1].setSpeed(600);
            motor[2].setSpeed(600);
            motor[3].setSpeed(600);

            printf("drone exit\n");
            break;
        }


    }//end while(1)

    sleep(3);
}//end int main()

