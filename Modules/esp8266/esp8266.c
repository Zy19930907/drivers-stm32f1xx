//
// Created by 周洋 on 2020/5/8.
//

#include "esp8266.h"

void mod_esp8266Init(_esp8266 *esp)
{
	esp->util = CreateDataUtil(DataUtil_UART,&esp->para);
}

void mod_esp8266Send(_esp8266 *esp)
{
	esp->util->write(esp->msg,esp->msgLen);
}