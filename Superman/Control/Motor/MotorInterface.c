/*
 * MoterInterface.h
 * @author  Jingyu Jung
 * @date    2015.10.07
 * 
 * MotorInterface 의 구현부로 동일한 인터페이스를 제공하기 위함.
 * Motor.h 에 의해 사용되는 장치가 결정되며 해당 파일을 인터페이스로 사용.
 * 새로운 장치를 추가하기 위해 이 파일을 복사하여 사용.
 */



#include "MoterInterface.h"

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

}


/*
 * calibration()
 * @function : Calbration for init setting ESC motor.
 * @detail   : 각 ESC 의 데이터시트 참조하여 Clibrate 코드 작성
 */
void Motor::calibration()
{

}


/*
 * setSpeed()
 * @function  : Set speed 
 * @ param    : Speed value, range ( 0 ~ 2500 )
 * @ detail   : 공통부분. Range 범위 내의 속도 지정
 */
void Motor::setSpeed(int value)
{

}



/*
 * stop()
 * @function : Stop drone
 * @detail   : 공통부분. 속도 0 지정 
 */
void Motor::stop()
{

} 
