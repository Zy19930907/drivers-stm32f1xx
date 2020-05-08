//
// Created by 周洋 on 2020/5/8.
//

#ifndef DRIVERS_STM32F1XX_TEMP_DS18B20_H
#define DRIVERS_STM32F1XX_TEMP_DS18B20_H

#include "module_opts.h"

typedef struct {
	_io dq;
	short temp;
}_ds18b20;

u8 mod_Ds18b20Init(_ds18b20 *ds18B20);
void mod_Ds18b20GetTemo(_ds18b20 *ds18B20);

#endif //DRIVERS_STM32F1XX_TEMP_DS18B20_H
