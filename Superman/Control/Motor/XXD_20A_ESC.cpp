/*
 * MoterInterface.h
 * @author  Jingyu Jung
 * @date    2015.10.07
 * 
 * MotorInterface 의 구현부로 동일한 인터페이스를 제공하기 위함.
 * Motor.h 에 의해 사용되는 장치가 결정되며 해당 파일을 인터페이스로 사용.
 * 새로운 장치를 추가하기 위해 이 파일을 복사하여 사용.
 */



#include "EMAX_30A_ESC.h"
#include <wiringPi.h>
#include <softPwm.h>

#include <stdio.h>

/*
 * Motor()
 * @function : Dafault consturctor
 */
Motor::Motor()
{
}


 /*
  * init()
  * @function : set up wiringPi enviroment
  * @return : -1 Error , 1 OK
  * @detail : 공통부분. wiringPi 설정 및 속도 초기화
  */
int Motor::init()
{
    speed = 0;
    
    if(wiringPiSetup() == -1)
        return -1;

    return 1;
}


/*
 * setPin()
 * @function : Selecting Pin number using softpwm output
 * @param    : pinNum, pin using pwm gpio
 * @return   : -1 Error, 1 OK
 * @detail   : 공통부분. 모터 사용 핀넘버 설정.
 */
int Motor::setPin(int pinNum)
{
    pin = pinNum;

    if(softPwmCreate(pin, 0, 2500) < 0)
        return -1;

    return 1;
}


/*
 * calibration()
 * @function : Calbration for init setting ESC motor.
 * @detail   : 각 ESC 의 데이터시트 참조하여 Clibrate 코드 작성
 */
void Motor::calibration()
{
    // NoESC needs not calibration

    printf("Start Cali!!\n");
    softPwmWrite(pin, 2200); //주기보다 최소 200us이상 작아야함.
    delay(4000); //최소 3500us이상 이어야함.

    printf("Finish Cali!!\n");
    softPwmWrite(pin, 800); //700us보다 50us이상 커야함
    delay(4000); //최소 3500us이상 이어야함.

}


/*
 * setSpeed()
 * @function  : Set speed 
 * @ param    : Speed value, range ( 800 ~ 2200 )
 * @ detail   : 공통부분. Range 범위 내의 속도 지정
 */
void Motor::setSpeed(int value)
{
  if(value < 800) value = 800;
  if(value > 2200) value = 2200;
    softPwmWrite(pin, value);
}

/*
 * stop()
 * @function : Stop drone
 * @detail   : 공통부분. 모터 정지 
 */
void Motor::stop()
{
    softPwmWrite(pin, 800);
} 
