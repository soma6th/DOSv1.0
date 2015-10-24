#include"BMP180.h"
#include<stdio.h>
#include<math.h>



BMP180::BMP180()
{
	wiringPiSetup();
	fd=wiringPiI2CSetup(BMP180_ADDR);
}


char BMP180::init()
{
	double c3,c4,b1;

	if(wiringPiI2CReadWord(fd,BMP180_ADDR,0xAA,(int16_t)&AC1) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xAC,(int16_t)&AC2) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xAE,(int16_t)&AC3) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xB0,(int16_t)&AC4) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xB2,(int16_t)&AC5) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xB4,(int16_t)&AC6) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xB6,(int16_t)&VB1) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xB8,(int16_t)&VB2) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xBA,(int16_t)&MB) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xBC,(int16_t)&MC) &&
		wiringPiI2CReadWord(fd,BMP180_ADDR,0xBE,(int16_t)&MD)
	  ){
		b1 = pow(160,2) * pow(2,-30) * VB1;
		c5 = (pow(2,-15) / 160) * AC5;
		c6 = AC6;
		mc = (pow(2,11) / pow(160,2)) * MC;
		md = MD / 160.0;
		x0 = AC1;
		x1 = 160.0 * pow(2,-13) * AC2;
		x2 = pow(160,2) * pow(2,-25) * VB2;
		y0 = c4 * pow(2,15);
		y1 = c4 * c3;
		y2 = c4 * b1;
		p0 = (3791.0 - 8.0) / 1600.0;
		p1 = 1.0 - 7357.0 * pow(2,-20);
		p2 = 3038.0 * 100.0 * pow(2,-36);

		return 1;
	}
	else
		return 0;
}


char BMP180::startTemperature()
{
	unsigned char data, result;
	data=BMP180_COMMAND_TEMPERATURE;
	result = wiringPiI2CWriteWord(fd,BMP180_ADDR,BMP180_REG_CONTROL,data);
	if (result) // good write?
		return(5); // return the delay in ms (rounded up) to wait before retrieving data
	else
		return(0); // or return 0 if there was a problem communicating with the BMP
}


char BMP180::getTemperature(double &T)
	// Retrieve a previously-started temperature reading.
	// Requires begin() to be called once prior to retrieve calibration parameters.
	// Requires startTemperature() to have been called prior and sufficient time elapsed.
	// T: external variable to hold result.
	// Returns 1 if successful, 0 if I2C error.
{
	unsigned char data;
	char result;
	double tu, a;

	result = wiringPiI2CReadByte(fd,BMP180_ADDR,BMP180_REG_RESULT,&data);
	if (result) // good read, calculate temperature
	{
		tu = (BMP180_REG_RESULT * 256.0) + data;

		//example from Bosch datasheet
		//tu = 27898;

		//example from http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf
		//tu = 0x69EC;

		a = c5 * (tu - c6);
		T = a + (mc / (a + md));

		/*		
				Serial.println();
				Serial.print("tu: "); Serial.println(tu);
				Serial.print("a: "); Serial.println(a);
				Serial.print("T: "); Serial.println(*T);
		 */
	}
	return(result);
}


char BMP180::startPressure(char oversampling)
	// Begin a pressure reading.
	// Oversampling: 0 to 3, higher numbers are slower, higher-res outputs.
	// Will return delay in ms to wait, or 0 if I2C error.
{
	unsigned char data, result, delay;

	switch (oversampling)
	{
		case 0:
			data = BMP180_COMMAND_PRESSURE0;
			delay = 5;
			break;
		case 1:
			data = BMP180_COMMAND_PRESSURE1;
			delay = 8;
			break;
		case 2:
			data = BMP180_COMMAND_PRESSURE2;
			delay = 14;
			break;
		case 3:
			data = BMP180_COMMAND_PRESSURE3;
			delay = 26;
			break;
		default:
			data = BMP180_COMMAND_PRESSURE0;
			delay = 5;
			break;
	}
	result = wiringPiI2CWriteByte(fd,BMP180_ADDR,BMP180_REG_CONTROL,data);
	if (result) // good write?
		return(delay); // return the delay in ms (rounded up) to wait before retrieving data
	else
		return(0); // or return 0 if there was a problem communicating with the BMP
}


char BMP180::getPressure(double &P, double &T)
{
	unsigned char data[2];
	char result;
	double pu,s,x,y,z;

	result = wiringPiI2CReadBytes(fd,BMP180_ADDR,BMP180_REG_RESULT,2,data);
	if (result) // good read, calculate pressure
	{
		pu = (BMP180_REG_RESULT * 256.0) + data[1] + (data[2]/256.0);


		s = T - 25.0;
		x = (x2 * pow(s,2)) + (x1 * s) + x0;
		y = (y2 * pow(s,2)) + (y1 * s) + y0;
		z = (pu - x) / y;
		P = (p2 * pow(z,2)) + (p1 * z) + p0;

	}
	return result;
}
