//
// Created by 周洋 on 2020/5/8.
//

#ifndef DRIVERS_STM32F1XX_ESP8266_H
#define DRIVERS_STM32F1XX_ESP8266_H

#include "module_opts.h"

typedef struct {
	_DataUtil *util;
	_uartpara para;
	u8 *msg;
	u16 msgLen;
}_esp8266;

#endif //DRIVERS_STM32F1XX_ESP8266_H
