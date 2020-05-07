//
// Created by 周洋 on 2020/3/29.
//

#include "bsp_f1xx_uart.h"
#include "bsp_f1xx_dma.h"

void Com1SendData(u8 *dat,u16 len);
void Com2SendData(u8 *dat,u16 len);

typedef void(*SendFunc)(u8 *dat,u16 len);
SendFunc SendFuncs[] = {Com1SendData,Com2SendData};

_uart uartlist[] = {
        {
                .pclk = 72000000,
                .irqn = USART1_IRQn,
                .uartclk = (vu32 *) &RCC->APB2ENR,
                .uartclkbit = BIT14,
                .uartx = USART1,

                .rxch = DMA1_Channel5,
                .txch = DMA1_Channel4,
                .dmarxirqn = DMA1_Channel5_IRQn,
                .dmatxirqn = DMA1_Channel4_IRQn,

//                .rxio = {PA,10,IO_MODE_IN_FLOAT},
//                .txio = {PA,9,IO_MODE_OUT_AF_PP_50MHZ},
                .rxio = {PB,7,IO_MODE_IN_FLOAT},
                .txio = {PB,6,IO_MODE_OUT_AF_PP_50MHZ},
        },
        {
                .pclk = 36000000,
                .irqn = USART2_IRQn,
                .uartclk = (vu32 *) &RCC->APB1ENR,
                .uartclkbit = BIT17,
                .uartx = USART2,

                .rxch = DMA1_Channel6,
                .txch = DMA1_Channel7,
                .dmarxirqn = DMA1_Channel6_IRQn,
                .dmatxirqn = DMA1_Channel7_IRQn,

                .rxio = {PA,3,IO_MODE_IN_FLOAT},
                .txio = {PA,2,IO_MODE_OUT_AF_PP_50MHZ},
        },
        {
                .pclk = 36000000,
                .irqn = USART3_IRQn,
                .uartclk = (vu32 *) &RCC->APB1ENR,
                .uartclkbit = BIT18,
                .uartx = USART3,

                .rxch = DMA1_Channel3,
                .txch = DMA1_Channel2,
                .dmarxirqn = DMA1_Channel3_IRQn,
                .dmatxirqn = DMA1_Channel2_IRQn,

                .rxio = {PB,11,IO_MODE_IN_FLOAT},
                .txio = {PB,10,IO_MODE_OUT_AF_PP_50MHZ},
        }
};

static inline void *uartmalloc(u32 size)
{
    return mymalloc(size);
}

static inline void uartfree(void *buf)
{
    myfree(buf);
}

static inline void InitComBuf(_uart *comx, u16 rxLen, u16 txLen)
{
    uartfree(comx->txbuf);
    uartfree(comx->rxbuf1);
    uartfree(comx->rxbuf2);

    comx->txbuf = uartmalloc(txLen);
    comx->rxbuf1 = uartmalloc(rxLen);
    comx->rxbuf2 = uartmalloc(rxLen);
    comx->rxbufLen = rxLen;
    comx->curbuf = comx->rxbuf1;
}

static inline u32 GetBaudDiv(u32 pclk, u32 baud)
{
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = ((float)(pclk)) / (baud << 4); //得到USARTDIV
    mantissa = temp;		      //得到整数部分
    fraction = (temp - mantissa) * 16;    //得到小数部分
    mantissa <<= 4;
    mantissa += fraction;
    return mantissa;
}

_DataUtil* bsp_UartInit(_uartpara *para)
{
    _DataUtil *util;
    _uart *cfguart = &uartlist[para->comx-1];

    cfguart->onDataRecved = para->recver;
    InitComBuf(cfguart,para->rxbuflen,para->txbuflen);

    dmainit(cfguart->rxch,(u32)&cfguart->uartx->DR,(u32)&cfguart->rxbuf1[0],0);
    dmainit(cfguart->txch,(u32)&cfguart->uartx->DR,(u32)&cfguart->txbuf[0],1);
    bsp_IoInit(&cfguart->txio);
    bsp_IoInit(&cfguart->rxio);
    RCC->APB2ENR |= BIT0; //AFIO时钟
    AFIO->MAPR |= BIT2;   //串口1重映射到PB6、PB7

    *cfguart->uartclk |= cfguart->uartclkbit;
    delay_ms(100);
    cfguart->uartx->BRR = GetBaudDiv(cfguart->pclk, para->baud); // 波特率设置
    cfguart->uartx->CR3 |= 0xC0;
    cfguart->uartx->CR1 |= 0X200C; //1位停止,无校验位.
    //使能空闲中断
    cfguart->uartx->CR1 |= BIT4;			   //idle中断
    MY_NVIC_Init(0,para->comx+2,cfguart->irqn,1);//组2，最低优先级
	MY_NVIC_Init(0, para->comx - 1, cfguart->dmarxirqn, 1);
    dma_stream_en(cfguart->rxch,cfguart->curbuf,cfguart->rxbufLen);

    util = uartmalloc(sizeof(_DataUtil));
    util->write = SendFuncs[para->comx-1];
    util->para = para;
    return util;
}

void bsp_UartClose(_DataUtil *util)
{
	_uartpara *para = (_uartpara*)util->para;
	_uart *cfguart = &uartlist[para->comx-1];
	uartfree(cfguart->rxbuf1);
	uartfree(cfguart->rxbuf2);
	uartfree(cfguart->txbuf);
	cfguart->rxch->CCR &= ~0x01;
}

static inline void OnComDataRecved(u8 comx,u8 isIdle)
{
    u8 *p = 0;
    u16 recvLen = 0;
    _uart *recvCom = &uartlist[comx - 1];

    if(isIdle)
        recvLen = recvCom->rxbufLen - recvCom->rxch->CNDTR;
    else
        recvLen = recvCom->rxbufLen;
    if(recvLen==0)
    {
        dma_clear_flag(recvCom->rxch);
        dma_stream_en(recvCom->rxch,recvCom->curbuf,recvCom->rxbufLen);
    }

    p = recvCom->curbuf;
    if(recvCom->curbuf == recvCom->rxbuf1)
    {
        recvCom->curbuf = recvCom->rxbuf2;
    }else{
        recvCom->curbuf = recvCom->rxbuf1;
    }
    dma_clear_flag(recvCom->rxch);
    dma_stream_en(recvCom->rxch,recvCom->curbuf,recvCom->rxbufLen);
    recvCom->onDataRecved(p, recvLen);
}

void USART1_IRQHandler(void)
{
    u8 temp = USART1->SR;
    if (temp & BIT4) //空闲中断
    {
        temp = USART1->DR;
        OnComDataRecved(COM1,1);
    }
}

void USART2_IRQHandler(void)
{
    u8 temp = USART2->SR;
    if (temp & BIT4) //空闲中断
    {
        temp = USART2->DR;
        OnComDataRecved(COM2,1);
    }
}

void ComxSendData(u8 comx, u8 *buf, u16 len)
{
    u16 i;

    _uart *com = &uartlist[comx - 1];
    if(len == 0 )
        return;
    if (!com->txch->CNDTR)
    {
        for (i = 0; i < len; i++)
            com->txbuf[i] = buf[i];
        dma_clear_flag(com->txch);
        dma_stream_en(com->txch,com->txbuf,len);
    }
}


// 往PC传输,将串口收到的数据搬移到缓冲区中
void DMA1_Channel6_IRQHandler(void)
{
	if ((DMA1->ISR) & BIT21) //缓冲区全满
	{
		DMA1->IFCR = 0x00F00000;
		OnComDataRecved(COM2,0);
	}
}

// 往PC传输,将串口收到的数据搬移到缓冲区中
void DMA1_Channel3_IRQHandler(void)
{
	if ((DMA1->ISR) & BIT9) //缓冲区全满
	{
		DMA1->IFCR = 0x00000F00;
		OnComDataRecved(COM3,0);
	}
}

void ComxSetBaud(u8 comx,u32 baud)
{
    _uart *com = &uartlist[comx - 1];
    com->uartx->BRR = GetBaudDiv(com->pclk, baud); // 波特率设置
    delay_ms(10);
}

void Com1SendData(u8 *dat,u16 len)
{
    ComxSendData(COM1,dat,len);
}

void Com2SendData(u8 *dat,u16 len)
{
    ComxSendData(COM2,dat,len);
}
