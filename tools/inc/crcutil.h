//
// Created by 周洋 on 2020/4/9.
//

#ifndef RECORDERCONFIGER_CRCUTIL_H
#define RECORDERCONFIGER_CRCUTIL_H

#include "public.h"

typedef struct
{
	void(*SetCrc)(u8 *dat,u16 len);
	u8(*CheckCrc)(u8 *dat,u16 len);
}_CrcUtil;
extern _CrcUtil CrcUtil;

#endif //RECORDERCONFIGER_CRCUTIL_H
