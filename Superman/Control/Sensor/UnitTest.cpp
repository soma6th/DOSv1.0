#define _MPU6050_
#include "Sensor.h"
#include "../PID/PID.h"
#include <wiringPi.h>


int main()
{

    int throttle = 0;

    PID PIDroll, PIDpitch, PIDyaw;

    PIDroll.init();
    PIDpitch.init();
    PIDyaw.init();

    
    // 상수 입력


    //
    IMU imu;

    float roll, pitch, yaw;

    imu.init();
    imu.calibration();

    printf("여기부터 진짜\n");


    while(1)
    {
        imu.getIMUData(&roll, &pitch, &yaw);
        printf("ROLL : %f, PITCH : %f, YAW : %f\n", roll, pitch, yaw);
    
    
    }
    






}
