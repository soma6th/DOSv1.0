#define __NOESC__
#define _MPU6050_

#include "Sensor/Sensor.h"
#include "Motor/Motor.h"
#include "PID/PID.h"
#include <stdio.h>

int main()
{
    int throttle = 300;

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
        return 0; //센서 연결 실패
    }

    PID rollPid, pitchPid, yawPid;

    rollPid.init();
    pitchPid.init();
    //yawPid.init();

    rollPid.setTuning(0.168, 0.0654, 0.008);
    pitchPid.setTuning(0.168, 0.0654, 0.008);
    //yawPid.setTuning(0.168, 0.0654, 0.008);

    float roll, pitch, yaw;
    int pidRoll, pidPitch, pidYaw;

    imu.calibration();

    printf("Start!!!\n");
    while(1)
    {
        imu.getIMUData(&roll, &pitch, &yaw);
        pidRoll = rollPid.calcPID(0, roll);
        pidPitch = pitchPid.calcPID(0, pitch);
        //pidYaw = yawPid.calcPID(0, yaw);

        int Motor1_speed = throttle + ( - pidRoll + pidPitch ) * 10;
        int Motor2_speed = throttle + ( + pidRoll + pidPitch ) * 10;
        int Motor3_speed = throttle + ( - pidRoll - pidPitch ) * 10;
        int Motor4_speed = throttle + ( + pidRoll - pidPitch ) * 10;


        //motor[0].setSpeed(Motor1_speed);
        //motor[1].setSpeed(Motor2_speed);
        //motor[2].setSpeed(Motor3_speed);
        //motor[3].setSpeed(Motor4_speed);

        printf("MOTOR 1 : %d  MOTOR 2 : %d  MOTOR 3 : %d  MOTOR 4 : %d   Roll : %.3f Pitch : %.3f Yaw : %.3f\n",
                Motor1_speed, Motor2_speed, Motor3_speed, Motor4_speed, roll, pitch, yaw);


    }




}

