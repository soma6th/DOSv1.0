#include<wiringPiI2C.h>
#include<wiringPi.h>
#include<stdint.h>

#define BMP180_ADDR 0x77
#define BMP180_REG_CONTROL 0xF4

#define BMP180_REG_RESULT 0xF6
#define BMP180_COMMAND_TEMPERATURE 0x2E

#define BMP180_COMMAND_PRESSURE0 0x34
#define BMP180_COMMAND_PRESSURE1 0x74
#define BMP180_COMMAND_PRESSURE2 0xB4
#define BMP180_COMMAND_PRESSURE3 0xF4


class BMP180
{
	public:
		BMP180();

		char init();

		char startTemperature(void);

		char getTemperature(double &T);

		char startPressure(char oversampling);

		char getPressure(double &P,double &T);

	//	void readData(double *P,double *T);

	private:
		int fd;
		int16_t AC1,AC2,AC3,VB1,VB2,MB,MC,MD;
		uint16_t AC4,AC5,AC6;
		double c5,c6,mc,md,x0,x1,x2,y0,y1,y2,p0,p1,p2;
		char _error;
};
