#define _MN2214_ESC_
#define _MPU6050_

#include "../Superman/Drone.h"
#include <stdio.h>
#include <signal.h>
#include <time.h>

// 1.2, 0.00084, 0.61340

int main(int argc, char* argv[])
{


    Motor motor[4];
       motor[0].init();
motor[1].init();
motor[2].init();
motor[3].init();
    

    motor[0].setPin(22);
    motor[1].setPin(23);
    motor[2].setPin(24);
    motor[3].setPin(25);

    motor[0].calibration();
    motor[1].calibration();
    motor[2].calibration();
    motor[3].calibration();

}//end int main()

