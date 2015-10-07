/*
 * wiringPiI2C.c:
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

/*
 * Notes:
 *	The Linux I2C code is actually the same (almost) as the SMBus code.
 *	SMBus is System Management Bus - and in essentially I2C with some
 *	additional functionality added, and stricter controls on the electrical
 *	specifications, etc. however I2C does work well with it and the
 *	protocols work over both.
 *
 *	I'm directly including the SMBus functions here as some Linux distros
 *	lack the correct header files, and also some header files are GPLv2
 *	rather than the LGPL that wiringPi is released under - presumably because
 *	originally no-one expected I2C/SMBus to be used outside the kernel -
 *	however enter the Raspberry Pi with people now taking directly to I2C
 *	devices without going via the kernel...
 *
 *	This may ultimately reduce the flexibility of this code, but it won't be
 *	hard to maintain it and keep it current, should things change.
 *
 *	Information here gained from: kernel/Documentation/i2c/dev-interface
 *	as well as other online resources.
 *********************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "wiringPi.h"
#include "wiringPiI2C.h"

/*
 * wiringPiI2CRead:
 *	Simple device read
 *********************************************************************************
 */

int wiringPiI2CRead(int fd)
{
  union i2c_smbus_data data ;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
    return -1 ;
  else
    return data.byte & 0xFF ;
}


/*
 * wiringPiI2CReadReg8: wiringPiI2CReadReg16:
 *	Read an 8 or 16-bit value from a regsiter on the device
 *********************************************************************************
 */

int wiringPiI2CReadReg8(int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
    return -1 ;
  else
    return data.byte & 0xFF ;
}

int wiringPiI2CReadReg16(int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data))
    return -1 ;
  else
    return data.word & 0xFFFF ;
}

int wiringPiI2CReadBit(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data)
{
  uint8_t b;
  int result = wiringPiI2CReadByte(fd, devAddr, regAddr, &b);

  *data = b & (1 << bitNum);

  return result;
}

int wiringPiI2CReadBitW(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data)
{
  uint16_t b;
  int result = wiringPiI2CReadWord(fd, devAddr, regAddr, &b);

  *data = b & (1 << bitNum);

  return result;
}

int wiringPiI2CReadBits(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
  uint8_t b;
  int result = wiringPiI2CReadByte(fd, devAddr, regAddr, &b);

  uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  b &= mask;
  b >>= (bitStart - length + 1);
  *data = b;

  return result;
}

int wiringPiI2CReadBitsW(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data)
{
  uint16_t w;
  int result = wiringPiI2CReadWord(fd, devAddr, regAddr, &w);

  uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  w &= mask;
  w >>= (bitStart - length + 1);
  *data = w;

  return result;
}

int wiringPiI2CReadByte(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t *data)
{
  return wiringPiI2CReadBytes(fd, devAddr, regAddr, 1, data);
}

int wiringPiI2CReadWord(int fd, uint8_t devAddr, uint8_t regAddr, uint16_t *data)
{
  uint8_t tmp_data[2];

  int result =  wiringPiI2CReadBytes(fd, devAddr, regAddr, 2, tmp_data);
  *data = tmp_data[0] << 8 | tmp_data[1];

  return result;
}

int wiringPiI2CReadBytes(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
  struct i2c_rdwr_ioctl_data dataR;
  struct i2c_msg msg[2];

  dataR.nmsgs = 2;
  dataR.msgs = msg;

  dataR.msgs[0].addr = devAddr;
  dataR.msgs[0].flags = 0; //Write Mode
  dataR.msgs[0].len = 1;
  dataR.msgs[0].buf = (char *)&regAddr;

  dataR.msgs[1].addr = devAddr;
  dataR.msgs[1].flags = 1; //Read Mode
  dataR.msgs[1].len = length;
  dataR.msgs[1].buf = (char *)data;

  return ioctl(fd, I2C_RDWR, &dataR);
}

/*
 * wiringPiI2CWrite:
 *	Simple device write
 *********************************************************************************
 */

int wiringPiI2CWrite (int fd, int data)
{
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL) ;
}


/*
 * wiringPiI2CWriteReg8: wiringPiI2CWriteReg16:
 *	Write an 8 or 16-bit value to the given register
 *********************************************************************************
 */

int wiringPiI2CWriteReg8(int fd, int reg, int value)
{
  union i2c_smbus_data data ;

  data.byte = value ;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data) ;
}

int wiringPiI2CWriteReg16(int fd, int reg, int value)
{
  union i2c_smbus_data data ;

  data.word = value ;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_WORD_DATA, &data) ;
}

int wiringPiI2CWriteBit(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    wiringPiI2CReadByte(fd, devAddr, regAddr, &b);

    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));

    return wiringPiI2CWriteByte(fd, devAddr, regAddr, b);
}

int wiringPiI2CWriteBitW(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data)
{
    uint16_t w;
    wiringPiI2CReadWord(fd, devAddr, regAddr, &w);

    w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));

    return wiringPiI2CWriteWord(fd, devAddr, regAddr, w);
}

int wiringPiI2CWriteBits(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    uint8_t b;
    wiringPiI2CReadByte(fd, devAddr, regAddr, &b);

    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    b &= ~(mask); // zero all important bits in existing byte
    b |= data; // combine data with existing byte

    return wiringPiI2CWriteByte(fd, devAddr, regAddr, b);
}

int wiringPiI2CWriteBitsW(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data)
{
    uint16_t w;
    wiringPiI2CReadWord(fd, devAddr, regAddr, &w);

    uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    w &= ~(mask); // zero all important bits in existing word
    w |= data; // combine data with existing word

    return wiringPiI2CWriteWord(fd, devAddr, regAddr, w);
}

int wiringPiI2CWriteByte(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
  return wiringPiI2CWriteBytes(fd, devAddr, regAddr, 1, &data);
}

int wiringPiI2CWriteWord(int fd, uint8_t devAddr, uint8_t regAddr, uint16_t data)
{
  uint8_t tmp_data[2];
  tmp_data[0] = data >> 8;
  tmp_data[1] = data & 0xFF;
  return wiringPiI2CWriteBytes(fd, devAddr, regAddr, 2, tmp_data);
}

int wiringPiI2CWriteBytes(int fd, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
  struct i2c_rdwr_ioctl_data dataW;
  struct i2c_msg msg[1];
  char write_data[length + 1]; //address byte + write byte

  dataW.nmsgs = 1;
  dataW.msgs = msg;

  dataW.msgs[0].addr = 0x68;
  dataW.msgs[0].flags = 0;
  dataW.msgs[0].len = sizeof(write_data);
  dataW.msgs[0].buf = write_data;
  dataW.msgs[0].buf[0] = regAddr;
  memcpy(dataW.msgs[0].buf + 1, data, length);

  return ioctl(fd, I2C_RDWR, &dataW);
}

/*
 * wiringPiI2CSetupInterface:
 *	Undocumented access to set the interface explicitly - might be used
 *	for the Pi's 2nd I2C interface...
 *********************************************************************************
 */

int wiringPiI2CSetupInterface (const char *device, int devId)
{
  int fd ;

  if ((fd = open (device, O_RDWR)) < 0)
    return wiringPiFailure (WPI_ALMOST, "Unable to open I2C device: %s\n", strerror (errno)) ;

  if (ioctl (fd, I2C_SLAVE, devId) < 0)
    return wiringPiFailure (WPI_ALMOST, "Unable to select I2C device: %s\n", strerror (errno)) ;

  return fd;
}


/*
 * wiringPiI2CSetup:
 *	Open the I2C device, and regsiter the target device
 *********************************************************************************
 */

int wiringPiI2CSetup (const int devId)
{
  int rev ;
  const char *device ;

  rev = piBoardRev () ;

  if (rev == 1)
    device = "/dev/i2c-0" ;
  else
    device = "/dev/i2c-1" ;

  return wiringPiI2CSetupInterface (device, devId) ;
}
