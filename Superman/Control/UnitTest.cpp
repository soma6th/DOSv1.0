#define __NOESC__
#define _MPU6050_

#include "Sensor/Sensor.h"
#include "Motor/Motor.h"
#include "PID/PID.h"
#include <stdio.h>

int main()
{
    int throttle = 300;

    IMU imu;

    imu.init();

    PID rollPid, pitchPid, yawPid;

    rollPid.init();
    pitchPid.init();
    yawPid.init();

    rollPid.setTuning(0.168, 0.0654, 0.008);
    pitchPid.setTuning(0.168, 0.0654, 0.008);
    yawPid.setTuning(0.168, 0.0654, 0.008);

    float roll, pitch, yaw;
    int pidRoll, pidPitch, pidYaw;

    imu.calibration();

    Motor motor[4];

    for(int i = 0 ; i < 4 ; i++)
    {
        motor[i].init();
    }

    motor[0].setPin(22);
    motor[1].setPin(23);
    motor[2].setPin(24);
    motor[3].setPin(25);

    printf("Start!!!\n");
    while(1)
    {
        imu.getIMUData(&roll, &pitch, &yaw);
        pidRoll = rollPid.calcPID(0, roll);
        pidPitch = pitchPid.calcPID(0, pitch);
        pidYaw = yawPid.calcPID(0, yaw);

        printf("MOTOR 1 OUTPUT : %d\n",throttle +(- pidRoll + pidPitch + pidYaw)*10);
        printf("MOTOR 2 OUTPUT : %d\n",throttle +(+ pidRoll + pidPitch - pidYaw)*10);
        printf("MOTOR 3 OUTPUT : %d\n",throttle +(- pidRoll - pidPitch - pidYaw)*10);
        printf("MOTOR 4 OUTPUT : %d\n",throttle +(+ pidRoll - pidPitch + pidYaw)*10);

        motor[0].setSpeed(throttle +(- pidRoll + pidPitch + pidYaw)*10);
        motor[1].setSpeed(throttle +(+ pidRoll + pidPitch - pidYaw)*10);
        motor[2].setSpeed(throttle +(- pidRoll - pidPitch - pidYaw)*10);
        motor[3].setSpeed(throttle +(+ pidRoll - pidPitch + pidYaw)*10);

    }




}
