//
// Created by 周洋 on 2020/3/29.
//

#ifndef STM32F1XX_BSP_F1XX_LED_H
#define STM32F1XX_BSP_F1XX_LED_H

#include "bsp_f1xx_io.h"

typedef struct {
    _io io;
    u8 lightlevel;
}_led;

void ledinit(_led *led);
void ledoff(_led *led);
void ledon(_led *led);
void ledfilp(_led *led);

#endif //STM32F1XX_BSP_F1XX_LED_H
