/*
 * HMC5883L.h
 *
 *  Created on: 2015. 11. 5.
 *      Author: wlsdn
 */

#ifndef SRC_HMC5883L_H_
#define SRC_HMC5883L_H_

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "HMC5883L_Registers.h"

class HMC5883L
{
public:
	HMC5883L();
    void initialize();
    int testConnection();

    // CONFIG_A register
    uint8_t getSampleAveraging();
    void setSampleAveraging(uint8_t averaging);
    uint8_t getDataRate();
    void setDataRate(uint8_t rate);
    uint8_t getMeasurementBias();
    void setMeasurementBias(uint8_t bias);

    // CONFIG_B register
    uint8_t getGain();
    void setGain(uint8_t gain);

    // MODE register
    uint8_t getMode();
    void setMode(uint8_t mode);

    // DATA* registers
    void getHeading(int16_t *x, int16_t *y, int16_t *z);
    int16_t getHeadingX();
    int16_t getHeadingY();
    int16_t getHeadingZ();

    // STATUS register
    bool getLockStatus();
    bool getReadyStatus();

    // ID_* registers
    uint8_t getIDA();
    uint8_t getIDB();
    uint8_t getIDC();

    void Calibration(int count = 300);
    void getYaw(int16_t mx, int16_t my, float *yaw);

private:
    int fd;
    float Heading_offset;
    uint8_t buffer[6];
    uint8_t mode;
};

#endif /* SRC_HMC5883L_H_ */
