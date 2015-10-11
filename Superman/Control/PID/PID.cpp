#include "PID.h"
#include <stdio.h>

PID::PID()
{
}


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
 
}


int PID::calcPID(float aimVal, float inVal)
{
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

void PID::setTuning(float kp, float ki, float kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}
