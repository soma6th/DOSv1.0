#define __NOESC__
#define _MPU6050_

#include "Sensor/Sensor.h"
#include "Motor/Motor.h"
#include "PID/PID.h"
#include <stdio.h>
#include <signal.h>

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

    kp = 0.61; // 0.61
    ki = 1.05; // 0.95
    kd = 0.56; // 0.26

    rollPid.setTuning(kp, ki, kd);
    pitchPid.setTuning(kp, ki, kd);
   // 0.61 , 0.09, 0.16
    //yawPid.setTuning(0.168, 0.0654, 0.008);

    float roll, pitch, yaw;
    int pidRoll, pidPitch, pidYaw;

    imu.calibration();

    char ch = '0';

    int Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed;
    printf("Start!!!\n");
    while(1)
    {
        imu.getIMUData(&roll, &pitch, &yaw);
        pidRoll = rollPid.calcPID(0, roll);
        pidPitch = pitchPid.calcPID(0, pitch);
        //pidYaw = yawPid.calcPID(0, yaw);

          Motor1_speed = throttle + ( + pidRoll - pidPitch )*12;
          Motor2_speed = throttle + ( + pidRoll + pidPitch )*12;
          Motor3_speed = throttle + ( - pidRoll + pidPitch )*12;
          Motor4_speed = throttle + ( - pidRoll - pidPitch )*12;
        
          motor[0].setSpeed(Motor1_speed); 
          motor[1].setSpeed(Motor2_speed);
          motor[2].setSpeed(Motor3_speed);
          motor[3].setSpeed(Motor4_speed);

        printf("[r = %f] [p = %f]  [PID = %f %f %f] MOTOR 1 : %d  MOTOR 2 : %d  MOTOR3 : %d  MOTOR4 : %d\n",
                roll, pitch, kp, ki, kd, Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed);


    }




}

