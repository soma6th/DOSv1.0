/*
 * MotorInterface
 * @author Jingyu Jung
 * @date 2015.10.07
 *
 * 모터 장치를 위한 기본 클래스 파일. 공통된 인터페이스를 제공하기 위해 사용.
 * 새로운 장치 추가시 이 파일을 include 하여 그대로 사용하도록 함. 
 * 각 함수들은 새로 추가하는 장치 환경에 따라 설정.
 */
class Motor {

    private :
        int speed;
        int pin;

    public :

        /*
         * Motor()
         * @function : Dafault consturctor
         */
        Motor();
        
        /*
         * init()
         * @function : set up wiringPi enviroment
         * @return : -1 Error , 1 OK
         */
        int init();

        /*
         * setPin()
         * @function : Selecting Pin number using softpwm output
         * @param    : pinNum, pin using pwm gpio
         * @return   : -1 Error, 1 OK
         */
        int setPin(int pinNum);

        /*
         * calibration()
         * @function : Calbration for init setting ESC motor. 
         */
        void calibration();
        
        /*
         * setSpeed()
         * @function : 
         * @ param    : Speed value, range ( 800 ~ 2200 )
         */
        void setSpeed(int value);

        /*
         * stop()
         * @function : Stop drone
         */
        void stop();
       

};
