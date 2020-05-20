//
// Created by 周洋 on 2020/5/8.
//

#ifndef DRIVERS_STM32F1XX_ESP8266_H
#define DRIVERS_STM32F1XX_ESP8266_H

#include "module_opts.h"

typedef enum {
	STA = 1,
	AP = 2,
	STA_AP = 3,
} _Esp8266Mode;

typedef struct {
	u8 ip1;
	u8 ip2;
	u8 ip3;
	u8 ip4;
	u16 port;
}_IpAddr;

typedef struct {
	_DataUtil *util;
	_uartpara para;
	u8 msg[1024];
	u8 waitAck[10];
	u8 ackLen;
	u16 msgLen;
	_IpAddr serverIp;
} _esp8266;

void mod_Esp8266Init(_esp8266 *esp);
void mod_Esp8266Rst(_esp8266 *esp);
void mod_Esp8266Send(_esp8266 *esp);
void mod_Esp8266CfgMode(_esp8266 *esp,_Esp8266Mode mode);
void mod_Esp8266LinkRouter(_esp8266 *esp,char *ssid,char *passwd);
void mod_Esp8266LinkServer(_esp8266 *esp);
void mod_Esp8266EnterTransfr(_esp8266 *esp);
void mod_Esp8266StartSend(_esp8266 *esp);

#endif //DRIVERS_STM32F1XX_ESP8266_H
