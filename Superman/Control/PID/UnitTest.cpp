#include "PID.h"
#include <stdio.h>

int main()
{
    PID pid;

    pid.init();
    pid.setTuning(0.168, 0.0654, 0.008);


    int current = 0;
    int prev = 0;

    
     printf("PID : %d\n", pid.calcPID(100, 0));
     printf("PID : %d\n", pid.calcPID(20, 10));
     printf("PID : %d\n", pid.calcPID(40, 30));    
     printf("PID : %d\n", pid.calcPID(149, 100));
     printf("PID : %d\n", pid.calcPID(120, 100));
    
     
     return 0;
}
