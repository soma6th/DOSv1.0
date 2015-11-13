/*
 *  LED.cpp
 *
 *  @author    Jingyu Jung
 *  @date      2015.11.14
 *
 *  일반적인 LED 장치 사용을 위한 클래스
 */

#include "LedInterface.h"
#include <wiringPi.h>

/*
 *  LED()
 *  @function : Default constructor
 */
LED::LED()
{
}

/*
 * setPin()
 * @function set up wirintPi envirment and set pin
 * @return : -1 Error, 1 OK
 */
int LED::setPin(int pinNum)
{
    wiringPiSetup();
    pin = pinNum;
    pinMode(pin, OUTPUT);
}

/*
 * on()
 * @function LED on
 */
void LED::on()
{
    digitalWrite(LED, HIGH);
}


/*
 * off()
 * @function LED off
 */
void LED::off()
{
    digitalWrite(LED, LOW);
}
