//
// Created on 2020/4/23.
//
#ifndef ZIGBEE_STM32_MASTER_FRAMEUTIL_H
#define ZIGBEE_STM32_MASTER_FRAMEUTIL_H

#include "public.h"

void OnFrameDataIn(u8 *dat, u16 len);
void SetFrameRecver(void *recver(u8 *buf,u8 len));
u16 MakeFrame(u8 *msg,u16 len);

#endif //ZIGBEE_STM32_MASTER_FRAMEUTIL_H
