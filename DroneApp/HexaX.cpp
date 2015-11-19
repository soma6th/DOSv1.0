/*
 *  HexaX.cpp
 *
 *  Data   : 2015.10.21
 *  Author : Jingyu Jung 
 *  Email  : monibu1548@gmail.com
 *
 *  PiDrone Application : 헥사콥터 x 형태 드론을 위한 어플리케이션
 */

// ESC 변속기 장치 설정
#define _MN2214_ESC_

// 자이로센서 장치 설정
#define _MPU6050_


// DronePi header include
#include "../Superman/Drone.h"

// etc include
#include <stdio.h>
#include <signal.h>
#include <time.h>



/*
 * main()
 *
 * @function DronePi Application main
 *
 */
int main(int argc, char* argv[])
{
    //yaw pid
    int count;
    char ff;

    int cnt=0;
    int ycnt=0;
    //network about
    int tcp,udp;
    int flag,str_len,t=0;
    double x = 0,y=0,z=0;
    double pp,pi,pd;
    char pidbuf[100],tcp_flag,recv_flag;
    json_t *data;
    json_error_t error;

    // Yaw PID 상수
    double yp,yi,yd;

    // Roll, Pitch PID 상수
    double kp, ki, kd;

    // 모터 출력량
    int throttle = 0;
    
    // 상대좌표 Yaw를 사용하기위한 보정 값
    float adjustYaw=0.f;

    // Hexacopter 로 모터 6개 선언
    Motor motor[6];

    // imu 장치 MPU6050 선언
    IMU imu;

    for(int i = 0 ; i < 6 ; i++)
    {
        motor[i].init();
    }

    // Setting Pin to Raspberry pi gpio
    motor[0].setPin(20);
    motor[1].setPin(21);
    motor[2].setPin(22);
    motor[3].setPin(23);
    motor[4].setPin(24);
    motor[5].setPin(25);

    
    // ESC Calibration
    motor[0].calibration();
    motor[1].calibration();  
    motor[2].calibration();
    motor[3].calibration();
    motor[4].calibration();
    motor[5].calibration();
    

    if(imu.init())
    {
        printf("Plese check the MPU6050 device connection!!\n");
        return 0; //센서 연결 실패
    }


    // PID 연산을 위한 각 축의 PID 연산자 객체 선언
    PID rollPid, pitchPid, yawPid;

    rollPid.init();
    pitchPid.init();
    yawPid.init();


    kp = 2.0; // 1.2
    ki = 0.1; // 0.00084
    kd = 0.85; // 0.6134

    rollPid.setTuning(2.0, 0.1, 0.78);
    pitchPid.setTuning(2.0, 0.1, 0.78);
    yawPid.setTuning(10.0, 0.f, 10.0);
  
    float roll, pitch, yaw, preYaw, rYaw;
    int pidRoll, pidPitch, pidYaw;

    // 자이로센서 init 및 calibration
    imu.calibration();

    int Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed, Motor5_speed, Motor6_speed;

    printf("Connect Application!!\n");

    // Network init
    if(network_init(&tcp,&udp)==-1)
    {
        printf("socket create error\n");
        return 0;
    }
    printf("network init complete\n");

    // 급격한 각도변화 발생시 비상제어. 착륙 실행 flag
    int Emergency = 0;

    // 상대 Yaw 좌표
    preYaw = 0;


    /*
     * Drone Control routine
     *
     * 1. 조작 데이터 수신
     * 2. 현재 각도 측정
     * 3. PID 연산
     * 4. 모터 속도 업데이트
     *
     */
    while(1)
    {
        // Get json data from android app
        flag=json_read(udp,&x,&y,&z,&t);
       

        if(flag==1)
        {
            throttle = t;
        }

        // 2. 현재 각도 측정
        imu.getIMUData(&roll, &pitch, &yaw);

        // 센서 오류 및 오동작으로 인한 동작 방지
        // 특정 각도 이상으로 기울어지면 종료할 수 있도록 함
        if( pitch < -50 || pitch > 50 || roll < -50 || roll > 50)
        {
            Emergency = 1;
        }

        // 상대 Yaw 연산
        if(preYaw>100&&yaw<-100)
            rYaw=360+yaw-preYaw;
        else if(preYaw<-100&&yaw>100)
            rYaw=360-yaw+preYaw;
        else
            rYaw= yaw - preYaw;

        if(ycnt==33)
        {
            printf("rYaw: %.2lf preYaw: %.2lf yaw: %.2lf\n",rYaw,preYaw,yaw);
            ycnt=0;
        }
        ycnt++;

        preYaw = yaw;

        // 각 축의 PID 연산
        pidRoll = rollPid.calcPID(x, roll);
        pidPitch = pitchPid.calcPID(-y, pitch);
        pidYaw = yawPid.calcPID(0, rYaw-z);

        // Hexacopter type X 의 모터 속도 연산
        Motor1_speed = throttle - 7/8*pidRoll + 1/2*pidPitch + pidYaw;
        Motor2_speed = throttle - 7/8*pidRoll - 1/2*pidPitch - pidYaw;
        Motor3_speed = throttle + 7/8*pidRoll + 1/2*pidPitch + pidYaw;
        Motor4_speed = throttle + 7/8*pidRoll - 1/2*pidPitch - pidYaw;
        Motor5_speed = throttle - pidPitch + pidYaw;
        Motor6_speed = throttle + pidPitch - pidYaw;

        if(cnt==33)
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
            // 긴급상황. 모터속도 최소화
            Motor1_speed = 700;
            Motor2_speed = 700;
            Motor3_speed = 700;
            Motor4_speed = 700;
        }

        if(Motor1_speed < 700) Motor1_speed = 700;
        if(Motor2_speed < 700) Motor2_speed = 700;
        if(Motor3_speed < 700) Motor3_speed = 700;
        if(Motor4_speed < 700) Motor4_speed = 700;

        // 모터 속도 업데이트
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
            if((json_unpack(data,"{s:f,s:f,s:f,s:f,s:f,s:f}","P_P",&pp,"P_I",&pi,"P_D",&pd,"Y_P",&yp,"Y_I",&yi,"Y_D",&yd))!=0)
            {
                printf("pid reset error\n");
                recv_flag=0;
                send(tcp,&recv_flag,1,MSG_DONTROUTE);
                return 0;
            }
            recv_flag=1;

            send(tcp,&recv_flag,1,MSG_DONTROUTE);

            rollPid.initKpid(pp, pi, pd);
            pitchPid.initKpid(pp, pi, pd);
            yawPid.initKpid(yp, yi, yd);
            Emergency=0;

            printf("pid reset complete\n");
        }
        else if(tcp_flag==5)
        {
            motor[0].setSpeed(700);
            motor[1].setSpeed(700);
            motor[2].setSpeed(700);
            motor[3].setSpeed(700);
            printf("drone exit\n");
            break;
        }
        else if(tcp_flag==0)
        {
            motor[0].setSpeed(700);
            motor[1].setSpeed(700);
            motor[2].setSpeed(700);
            motor[3].setSpeed(700);
            printf("Controller connection less\n");
            break;
        }


    }//end while(1)

    sleep(3);
}//end int main()

