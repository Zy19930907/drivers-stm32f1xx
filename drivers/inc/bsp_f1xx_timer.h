//
// Created by 周洋 on 2020/3/29.
//

#ifndef STM32F1XX_BSP_F1XX_TIMER_H
#define STM32F1XX_BSP_F1XX_TIMER_H

#include "public.h"
typedef enum{
    tim1 = 0,
    tim2,
    tim3,
    tim4,
    tim5,
    tim6,
    tim7,
    tim8,
}_timx;

typedef struct {
    TIM_TypeDef *timx;
    u8 sec;
    u16 ms;
    void(*ontimout)(void);
}_timer;
void bsp_TimerInit(_timer *timer);

#endif //STM32F1XX_BSP_F1XX_TIMER_H
