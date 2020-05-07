//
// Created by 周洋 on 2020/3/29.
//

#include "bsp_f1xx_dma.h"

DMA_TypeDef *getdmax(DMA_Channel_TypeDef *chx)
{
	if (chx > DMA2_Channel1)
		return DMA2;
	else
		return DMA1;
}

//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量
void dmainit(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u8 dir)
{
	if (DMA_CHx >= DMA2_Channel1) {
		RCC->AHBENR |= 0x02;
		while (!(RCC->AHBENR & 0x02));
	} else {
		RCC->AHBENR |= 0x01;
		while (!(RCC->AHBENR & 0x01));
	}

	DMA_CHx->CPAR = cpar;      //DMA1 外设地址
	DMA_CHx->CMAR = (u32) cmar; //DMA1,存储器地址
	DMA_CHx->CCR = 0X00000000; //复位
	DMA_CHx->CCR |= dir << 4;    //从存储器读
	DMA_CHx->CCR |= 0 << 5;    //普通模式
	DMA_CHx->CCR |= 0 << 6;    //外设地址非增量模式
	DMA_CHx->CCR |= 1 << 7;    //存储器增量模式
	DMA_CHx->CCR |= 0 << 8;    //外设数据宽度为8位
	DMA_CHx->CCR |= 0 << 10;   //存储器数据宽度8位
	DMA_CHx->CCR |= 1 << 12;   //中等优先级
	DMA_CHx->CCR |= 0 << 14;   //非存储器到存储器模式
}

//开启一次DMA传输
void dma_stream_en(DMA_Channel_TypeDef *chx, u8 *buf, u16 len)
{
	chx->CCR &= ~(1 << 0);     //关闭DMA传输
	chx->CMAR = (u32) &buf[0];
	chx->CNDTR = len;
	chx->CCR |= 1 << 0;           //开启DMA传输
}

void dma_clear_flag(DMA_Channel_TypeDef *chx)
{
	DMA_TypeDef *dmax = getdmax(chx);
	if (dmax == DMA1)
		dmax->IFCR = (u32) 0x0F << ((((u32) chx - DMA1_Channel1_BASE) / 0x14) << 2);
	else
		dmax->IFCR = (u32) 0x0F << ((((u32) chx - DMA2_Channel1_BASE) / 0x14) << 2);
}
