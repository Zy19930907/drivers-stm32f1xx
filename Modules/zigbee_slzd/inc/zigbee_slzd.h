//
// Created by 周洋 on 2020/5/8.
//

#ifndef DRIVERS_STM32F1XX_ZIGBEE_SLZD_H
#define DRIVERS_STM32F1XX_ZIGBEE_SLZD_H

#include "module_opts.h"

#define MAXZIGBEEPACKLEN					64

typedef struct {
	u8 pack[MAXZIGBEEPACKLEN];	//接收数据缓存
	u8 temp[MAXZIGBEEPACKLEN];	//处理数据缓存
	u8 headRecved;								//是否接收到帧头
	u8 recvLen;									//当前接收数据长度
	u8 needNextByte;							//是否接收到转义字符

	u8 msg[MAXZIGBEEPACKLEN];		//发送数据缓存
	u8 msgLen;									//需要发送数据长度
	u16 desAddr;									//数据发送目标地址
	u8 srcPort;									//源端口
	u8 desPort;									//目的端口

	_DataUtil *util;
	_uartpara para;
	void(*packRecver)(u8 srcPort, u8 desPort, u16 remoteAddr, u8 *buf, u8 len);
}_zgb_slzd;

#endif //DRIVERS_STM32F1XX_ZIGBEE_SLZD_H
