#include "PID.h"

PID::PID()
{
}

PID::PID(double kp, double ki, double kd)
{
    Kd = kd;
    Ki = ki;
    Kp = kp;
}

int PID::init()
{
    Kd = 0;
    Ki = 0;
    Kp = 0;

    P_pid = 0;
    I_pid = 0;
    D_pid = 0;

    dt = 0.01;
    P_pid_prev = 0;
    output = 0;
}


double PID::calcPID(double aimVal, double inVal)
{
    double output;

    P_pid = aimVal - inVal;

    I_pid += P_pid * dt;

    D_pid = ( P_pid - P_pid_prev ) / dt;


    P_pid_prev = P_pid;

    output = ( Kp * P_pid ) + ( Ki * I_pid ) + ( Kd * D_pid );
    
    return output;
}
void setTuning(double kp, double, ki, double kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}
