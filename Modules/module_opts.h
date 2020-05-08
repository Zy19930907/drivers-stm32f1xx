//
// Created by 周洋 on 2020/5/8.
//

#ifndef DRIVERS_STM32F1XX_MODULE_OPTS_H
#define DRIVERS_STM32F1XX_MODULE_OPTS_H

#include "bsp_f1xx_io.h"
#include "bsp_f1xx_uart.h"

void mod_delay_us(u32 nus);
void mod_delay_ms(u32 nms);
void mod_IoSetDir(_io *io,u8 dir);
void mod_IoSetVal(_io *io,u8 val);
u8 mod_IoGetVal(_io *io);
void mod_IoInit(_io *io);

#endif //DRIVERS_STM32F1XX_MODULE_OPTS_H
