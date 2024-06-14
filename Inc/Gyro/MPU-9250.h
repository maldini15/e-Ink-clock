#ifndef MPU9250_H_
#define MPU9250_H_

#include "stdint.h"

#define MPU6500I2CADDR 0x68U

#define REG_SMPLRT_DIV
#define REG_CONFIG
#define REG_GYRO_CONFIG
#define REG_ST_Y_GYRO
#define REG_ST_X_GYRO
#define REG_ST_Z_GYRO
#define GYRO_CONFIG_FS_POS 3
#define
#define
#define

void initGyro();

void deInit();

void setSleepMode();

uint16_t getRoll();

#endif
