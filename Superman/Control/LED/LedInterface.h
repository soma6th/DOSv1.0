/*
 * LedInterface
 * @author Jingyu Jung
 * @date 2015.11.14
 *
 * LED 장치를 위한 기본 클래스 파일. 공통된 인터페이스를 제공하기 위해 사용.
 * 새로운 장치 추가시 이 파일을 include 하여 그대로 사용하도록 함. 
 * 각 함수들은 새로 추가하는 장치 환경에 따라 설정.
 */
class LED {

    private :
        int pin;

    public :

        /*
         * LED()
         * @function : Dafault consturctor
         */
        LED();
    
        /*
         * setPin()
         * @function : Selecting Pin number
         * @param    : pinNum, pin using gpio
         * @return   : -1 Error, 1 OK
         */
        int setPin(int pinNum);

        /*
         * on
         * @function led on. 
         */
        void on();
        
        /*
         * off()
         * @function led off 
         */
        void off();


}
