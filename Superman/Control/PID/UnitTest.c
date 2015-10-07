#include <stdio.h>
#include "PID.h"


int main()
{

    PID pid;
    pid.setTuning(2.6, 0.011, 6);

    int current = 0;
    int prev = 0;
    int loop = 0;


    for(loop = 0; loop < 10000; loop++)
    {
        current = loop * 2 % 445;
        printf("PID OUTPUT : %d\n", pid.calcPID(prev, current));
        prev=current;
    }
    


    return 0;
}
