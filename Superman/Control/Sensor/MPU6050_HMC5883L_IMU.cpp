/*
 * MPU6050_HMC5883L_IMU.cpp
 *
 *  Created on: 2015. 11. 6.
 *      Author: wlsdn
 */

#include "MPU6050_HMC5883L_IMU.h"

int IMU::init()
{
	mag.initialize();
	mpu.initialize();
	if(!mpu.testConnection() && !mpu.dmpInitialize() &&
		 !mag.testConnection())
	{
		return 0; //successful!
	}
	else return 1; //Error!
}

int IMU::getIMUData(float *roll, float *pitch, float *yaw)
{
	mpu.setDMPEnabled(true);

	uint16_t packetSize = mpu.dmpGetFIFOPacketSize();
	while(mpu.getFIFOCount() < packetSize); //DMP Data가 들어 올때 까지 대기.

	uint8_t mpuIntStatus = mpu.getIntStatus();
	if(mpuIntStatus & 0x02)
    {
    	uint8_t fifoBuffer[64];
        VectorFloat gravity;    // [x, y, z]
        Quaternion q;           // [w, x, y, z]
        float ypr[3];           // [yaw, pitch, roll]

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
    	mpu.setDMPEnabled(false);

        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        *roll = ypr[2] * 180 / M_PI;
        *pitch = ypr[1] * 180 / M_PI;
        //*yaw = ypr[0] * 180 / M_PI;

        int16_t mx, my, mz;
        mag.getHeading(&mx, &my, &mz);
        mag.getYaw(mx, my, yaw);

        //if(*yaw < 0) *yaw += 2 * M_PI;


        return 0;
    }
	else if((mpuIntStatus & 0x10) || (mpu.getFIFOCount() == 1024))
	{
	   cout << "FIFO overflow!" << endl;
	   mpu.resetFIFO();
	}

	mpu.setDMPEnabled(false);
	return 1;
}

void IMU::calibration()
{
	mag.Calibration();
	mpu.Calibration();
}
