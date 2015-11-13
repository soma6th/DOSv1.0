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
	 * @function : WiringPi, MPU6050 초기화, 연결확인.
	 * @return   : 1  = 연결 실패, 0 = 연결 성공
	 * @detail   : 공통부분
	 */
	int init();

	/*
	 * @function : 센서 offest 보정
	 * @detail   : 공통부분
	 */
	void calibration();

	/*
	 * @function : MPU6050의 DMP를 통해 Roll, Pitch, Yaw를 계산.
	 * @return	 : 1 = 실패, 0 = 성공
	 * @detail   : 공통부분
	 */
	int getIMUData(float *roll, float *pitch, float *yaw);

	/*
	 * @function : 자이로 센서와 가속도센서의 offset 읽기
	 * @param
	 * 		gx = Gyro X axis offset
	 * 		gy = Gyro Y axis offset
	 * 		gz = Gyro Z axis offset
	 * 		ax = Accel X axis offset
	 * 		ay = Accel Y axis offset
	 * 		az = Accel Z axis offset
	 * @detail   : 공통부분
	 */
	void getIMUOffset(int16_t *gx,int16_t *gy, int16_t *gz, int16_t *ax, int16_t *ay, int16_t *az);

	/*
	 * @function : 자이로 센서와 가속도센서의 offset 설정
	 * @param
	 * 		gx = Gyro X axis offset
	 * 		gy = Gyro Y axis offset
	 * 		gz = Gyro Z axis offset
	 * 		ax = Accel X axis offset
	 * 		ay = Accel Y axis offset
	 * 		az = Accel Z axis offset
	 * @detail   : 공통부분
	 */
	void setIMUoffset(int16_t gx,int16_t gy, int16_t gz, int16_t ax, int16_t ay, int16_t az);
};


#endif /* SENSORINTERFACE_H_ */
