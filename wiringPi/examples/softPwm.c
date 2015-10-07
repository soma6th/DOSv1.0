#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define LED 0       //BCM_GPIO 18

int main(void)
{
    int loop;

    if(wiringPiSetup() == -1)
        return 1;

    softPwmCreate(LED,0,2500);

        
    printf("Set\n");


    for(loop = 0; loop < 1000; loop++)
    {
        softPwmWrite(LED, 375);
        delay(10);
    }

    printf("Zero\n");

for(loop = 0; loop < 1000; loop++)
    {
        softPwmWrite(LED, 0);
        delay(10);
    }

    printf("Cali\n");

    
    for(loop = 140  ; loop < 2500; loop++)
    {
        softPwmWrite(LED, loop);
        printf("POWER LEVEL : %d\n", loop);
        delay(2000);
    }
    
    softPwmWrite(LED, 0);
    delay(10);
}
