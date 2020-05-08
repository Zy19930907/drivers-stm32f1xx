//
// Created by 周洋 on 2020/5/8.
//

#ifndef DRIVERS_STM32F1XX_TEMP_DHT11_H
#define DRIVERS_STM32F1XX_TEMP_DHT11_H

#include "module_opts.h"

typedef struct {
	_io dq;
	u8 temp;
	u8 hump;
}_dht11;

u8 mod_Dht11Init(_dht11 *dht11);
u8 mod_Dht11Read(_dht11  *dht11,u8 *temp,u8 *humi);

#endif //DRIVERS_STM32F1XX_TEMP_DHT11_H
