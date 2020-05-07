//
// Created by 周洋 on 2020/5/7.
//

#ifndef DRIVERS_STM32F1XX_BSP_SOFT_I2C_H
#define DRIVERS_STM32F1XX_BSP_SOFT_I2C_H
#include "bsp_f1xx_io.h"

typedef struct {
	_io sda;
	_io scl;
}_I2C;

void bsp_SoftI2CInit(_I2C *i2c);
u8 bsp_SoftI2CReadReg(_I2C *i2c,u8 addr,u8 reg);
s16 bsp_SoftI2CWriteReg(_I2C *i2c,u8 addr,u8 reg,u8 dat);
s16 bsp_SoftI2CWrite(_I2C *i2c, u8 addr, u8 reg, u8 *dat, u16 len);
s16 bsp_SoftI2CRead(_I2C *i2c,u8 addr,u8 reg,u8 *dat,u16 len);

#endif //DRIVERS_STM32F1XX_BSP_SOFT_I2C_H
