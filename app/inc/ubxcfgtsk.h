//
// Created by 周洋 on 2020/3/31.
//

#ifndef RECORDERCONFIGER_UBXCFGTSK_H
#define RECORDERCONFIGER_UBXCFGTSK_H

#include "public.h"

typedef enum{
	UBXCFGIDLE,
	UBXCFGRECVED,
	UBXCFGWAITACK,
}_UbxCfgStatus;

typedef struct{
	_UbxCfgStatus status;
	u8 cfgFlag;
	u8 cmd[300];
	u16 cmdLen;
}_UbxCfger;

void StartUbxCfgTask(void);

#endif //RECORDERCONFIGER_UBXCFGTSK_H
