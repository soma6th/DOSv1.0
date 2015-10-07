/*
 * wiringPiI2C.h:
 *	Simplified I2C access routines
 *	Copyright (c) 2013 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern int wiringPiI2CRead           (int fd) ;
extern int wiringPiI2CReadReg8       (int fd, int reg) ;
extern int wiringPiI2CReadReg16      (int fd, int reg) ;

extern int wiringPiI2CWrite          (int fd, int data) ;
extern int wiringPiI2CWriteReg8      (int fd, int reg, int data) ;
extern int wiringPiI2CWriteReg16     (int fd, int reg, int data) ;

extern int wiringPiI2CSetupInterface (const char *device, int devId) ;
extern int wiringPiI2CSetup          (const int devId) ;


extern int wiringPiI2CReadBit 		 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
extern int wiringPiI2CReadBitW 		 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data);
extern int wiringPiI2CReadBits 		 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
extern int wiringPiI2CReadBitsW 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data);
extern int wiringPiI2CReadByte 		 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t *data);
extern int wiringPiI2CReadWord 		 (int fd, uint8_t devAddr, uint8_t regAddr, uint16_t *data);
extern int wiringPiI2CReadBytes 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);

extern int wiringPiI2CWriteBit 		 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
extern int wiringPiI2CWriteBitW 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
extern int wiringPiI2CWriteBits 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
extern int wiringPiI2CWriteBitsW 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
extern int wiringPiI2CWriteByte 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t data);
extern int wiringPiI2CWriteWord 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint16_t data);
extern int wiringPiI2CWriteBytes 	 (int fd, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);

#ifdef __cplusplus
}
#endif