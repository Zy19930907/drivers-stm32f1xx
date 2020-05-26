//
// Created on 2020/4/23.
//
#ifndef ZIGBEE_STM32_MASTER_FRAMEUTIL_H
#define ZIGBEE_STM32_MASTER_FRAMEUTIL_H

#include "public.h"

#define FRAMEBUFSIZE			1024
void OnFrameDataIn(u8 *dat, u16 len);
void SetFrameRecver(void (*recver)(u16 clas,u8 *buf,u16 len));
u16 MakeFrame(u16 class,u8 *msg,u16 len);

#endif //ZIGBEE_STM32_MASTER_FRAMEUTIL_H
