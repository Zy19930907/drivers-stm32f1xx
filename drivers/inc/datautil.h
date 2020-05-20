//
// Created by 周洋 on 2020/3/31.
//

#ifndef RECORDERCONFIGER_DATAUTIL_H
#define RECORDERCONFIGER_DATAUTIL_H

#include "public.h"

#define COM1 1
#define COM2 2
#define COM3 3
#define COM4 4

typedef struct{
    u8 comx;
    u32 baud;
    u16 rxbuflen;
    u16 txbuflen;
    void(*recver)(u8 *dat,u16 len);
}_uartpara;

typedef enum{
    DATAUTIL_UART,			//硬件串口
    DATAUTIL_SPI,				//硬件SPI
    DATAUTIL_SOFTI2C,		//软件模拟I2C
    DATAUTIL_SOFTSPI,		//软件模拟SPI
}_DataUtilType;

typedef struct{
	void *para;
    void(*write)(u8 *msg,u16 len);
    u16(*read)(u8 *msg,u16 len);
    void(*close)(void);
}_DataUtil;

_DataUtil* CreateDataUtil(_DataUtilType type,void *args);
void CloseDataUtil(_DataUtilType type,_DataUtil *util);


#endif //RECORDERCONFIGER_DATAUTIL_H
