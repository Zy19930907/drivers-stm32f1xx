//
// Created by 周洋 on 2020/3/29.
//

#ifndef STM32F1XX_BSP_F1XX_DMA_H
#define STM32F1XX_BSP_F1XX_DMA_H

#include "public.h"
void dma_clear_flag(DMA_Channel_TypeDef *chx);
void dma_stream_en(DMA_Channel_TypeDef *chx,u8 *buf,u16 len);
void dmainit(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar,u8 dir);

#endif //STM32F1XX_BSP_F1XX_DMA_H
