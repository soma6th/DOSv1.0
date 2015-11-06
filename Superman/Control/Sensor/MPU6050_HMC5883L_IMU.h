/*
 * MPU6050_HMC5883L_IMU.h
 *
 *  Created on: 2015. 11. 6.
 *      Author: wlsdn
 */

#ifndef SRC_MPU6050_HMC5883L_IMU_H_
#define SRC_MPU6050_HMC5883L_IMU_H_

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "MPU6050/MPU6050.h"
#include "HMC5883L/HMC5883L.h"

using namespace std;

class IMU
{
public:
	/*
	 * init() :
	 * 		WiringPi I2C, MPU6050, HMC5881초기화, 연결확인.
	 * return :
	 * 		1  = 연결 실패, 0 = 연결 성공
	 */
	int init();

	/*
	 * getIMUData(float *roll, float *pitch, float *yaw) :
	 * 		MPU6050의 DMP와 HMC5881을 통해 Roll, Pitch, Yaw를 계산.
	 * 	return :
	 * 		1 = 실패, 0 = 성공
	 */
	int getIMUData(float *roll, float *pitch, float *yaw);

	/*
	 * void calibration() :
	 * 		센서 offest 보정
	 */
	void calibration();

private:
	MPU6050 mpu;
	HMC5883L mag;
};


#endif /* SRC_MPU6050_HMC5883L_IMU_H_ */
