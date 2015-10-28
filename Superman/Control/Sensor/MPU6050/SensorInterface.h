/*
 * SensorInterface.h
 *
 *  Created on: 2015. 10. 7.
 *      Author: wlsdn
 */

#ifndef SENSORINTERFACE_H_
#define SENSORINTERFACE_H_

/*
 * IMU의 기본적인 함수들을 정의 한 부분으로, 새로운 센서를 추가할때 IMUInterface 클래스를 상속 받아 코딩할 것.
 */

class IMUInterface
{
public:
	/*
	 * init() :
	 * 		센서  초기화, 연결확인.
	 * return :
	 * 		1  = 연결 성공 , 0 = 연결 실패
	 */
	int init();

	/*
	 * getIMUData(float *roll, float *pitch, float *yaw) :
	 * 		센서의 Roll, Pitch, Yaw를 계산.
	 */
	void getIMUData(float *roll, float *pitch, float *yaw);

	/*
	 * void calibration() :
	 * 		센서 offest 보정
	 */
	void calibration();
};


#endif /* SENSORINTERFACE_H_ */
