#include "PID.h"
#include <stdio.h>

PID::PID()
{
}

/*
 *  init()
 *  @function init member variables
 *  @param void 
 *  @return void
 */
int PID::init()
{

    Kd = 0.f;
    Ki = 0.f;
    Kp = 0.f;

    P_pid = 0.f;
    I_pid = 0.f;
    D_pid = 0.f;

    P_err = 0.f;
    I_err = 0.f;
    D_err = 0.f;

    dt = 0.01f;
    P_pid_prev = 0.f;

    gettimeofday(&start_point, NULL);
    gettimeofday(&end_point, NULL);

}

/*
 *  calcPID(float, float)
 *  @function Calculation pid ( input to aim value )
 *  @param 
 *      aimVal : aim that we want tilt
 *      inVal  : current tilt
 *  @return
 *      int : integer value that motor power
 */
int PID::calcPID(float aimVal, float inVal)
{
    gettimeofday(&end_point, NULL);

    dt = (double)(end_point.tv_sec)+(double)(end_point.tv_usec)/1000000.0-(double)(start_point.tv_sec)-(double)(start_point.tv_usec)/1000000.0;

    if(dt > 1) dt = 0.001;

    gettimeofday(&start_point, NULL);

    int output;

    P_err = aimVal - inVal;
    P_pid = P_err * Kp;

    I_err += P_pid * dt;
    I_pid = I_err * Ki;

    D_err = ( P_pid - P_pid_prev ) / dt;
    D_pid = D_err * Kd;


    P_pid_prev = P_pid;


    //    printf("[p] = %f, [I] = %f, [D] = %f\n", P_pid, I_pid, D_pid);
    output =P_pid + I_pid  +  D_pid; 

    return output;
}

/*
 *  setTuning(float, float, float)
 *  @function set pid constant for calculating 
 *  @param
 *      kp : p constant
 *      ki : i constant
 *      kd : d constant
 * 
 */

void PID::setTuning(float kp, float ki, float kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

/*
 *  initKpid(float, float, float)
 *  @function changing pid constant during executing program
 *  
 */
void PID::initKpid(float kp, float ki, float kd)
{
    init();
    setTuning(kp, ki, kd);
}
