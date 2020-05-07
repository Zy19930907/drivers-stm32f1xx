//
// Created by 周洋 on 2020/3/29.
//
#ifndef STM32F1XX_BSP_F1XX_UART_H
#define STM32F1XX_BSP_F1XX_UART_H

#include "public.h"
#include "bsp_f1xx_io.h"
#include "datautil.h"

typedef struct{
    u32 pclk;
    u8 irqn;

    u16 rxbufLen;
    u8 *curbuf;
    u8 *rxbuf1;
    u8 *rxbuf2;
    u8 *txbuf;
    vu32 *uartclk;
    u32 uartclkbit;
    USART_TypeDef *uartx;

    u8 dmarxirqn;
    u8 dmatxirqn;
    DMA_Channel_TypeDef *rxch;
    DMA_Channel_TypeDef *txch;

    _io txio;
    _io rxio;
    void (*onDataRecved)(u8 *dat, u16 len);
}_uart;

_DataUtil* bsp_UartInit(_uartpara *para);
void ComxSetBaud(u8 comx,u32 baud);
void ComxSendData(u8 comx, u8 *buf, u16 len);
void bsp_UartClose(_DataUtil *util);

#endif //STM32F1XX_BSP_F1XX_UART_H
