//
// Created by 周洋 on 2020/3/29.
//

#include "bsp_f1xx_timer.h"

#define TIMCLK  72

typedef void (*timoutfunc)(void);
static timoutfunc funcs[6];
static vu64 psc;
static vu64 arr;
u8 timirqs[] = {TIM2_IRQn,TIM3_IRQn,TIM4_IRQn,TIM5_IRQn,TIM6_IRQn,TIM7_IRQn};

static inline void caltimcfg(_timer *timer)
{
    psc = 0;
    arr = 0;
    u64 tim;
    u64 arrmax=65535;
    u64 timclk = TIMCLK*1000;

    tim = ((timer->sec * 1000) + timer->ms);
    while(psc<65535)
    {
        if((timclk%(psc+1))!=0)
            psc++;
        else
        {
            arr = ((timclk / (psc+1)))*tim-1;
            if(arr >= arrmax)
                psc++;
            else
                return;
        }
    }
}

void bsp_TimerInit(_timer *timer)
{
    u8 offset;

    TIM_TypeDef *cfgtim = timer->timx;
    offset=	((((u32) timer->timx) - TIM2_BASE) / 0x400);//计算时钟位置偏移量
    caltimcfg(timer);

    RCC->APB1ENR|=1<<offset;				//时钟使能

    cfgtim->ARR= arr;  				//设定计数器自动重装值
    cfgtim->PSC= psc;  				//预分频器
    cfgtim->SMCR=0;					//SMS[3:0]=0000,CK_INT作为时钟源
    cfgtim->DIER|=1<<0;  			//允许更新中断
    cfgtim->CR1&=~(3<<5);			//CMS[1:0]=00,边沿对齐模式
    cfgtim->CR1&=~(1<<4);			//DIR=0,向上计数模式

    funcs[offset] = timer->ontimout;
    MY_NVIC_Init(1,offset,timirqs[offset],2);	//抢占1，子优先级3，组2
    cfgtim->CR1|=0x01;				//使能定时器3
}

//定时器2中断服务程序
void TIM2_IRQHandler(void)
{
    if(TIM2->SR&0X0001)//溢出中断
    {
        funcs[0]();
    }
    TIM2->SR&=~(1<<0);//清除中断标志位
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)
{
    if(TIM3->SR&0X0001)//溢出中断
    {
        funcs[1]();
    }
    TIM3->SR&=~(1<<0);//清除中断标志位
}

//定时器4中断服务程序
void TIM4_IRQHandler(void)
{
    if(TIM4->SR&0X0001)//溢出中断
    {
        funcs[2]();
    }
    TIM4->SR&=~(1<<0);//清除中断标志位
}

//定时器5中断服务程序
void TIM5_IRQHandler(void)
{
    if(TIM5->SR&0X0001)//溢出中断
    {
        funcs[3]();
    }
    TIM5->SR&=~(1<<0);//清除中断标志位
}

//定时器6中断服务程序
void TIM6_DAC_IRQHandler(void)
{
    if(TIM6->SR&0X0001)//溢出中断
    {
        funcs[4]();
    }
    TIM4->SR&=~(1<<0);//清除中断标志位
}

//定时器7中断服务程序
void TIM7_IRQHandler(void)
{
    if(TIM7->SR&0X0001)//溢出中断
    {
        funcs[5]();
    }
    TIM7->SR&=~(1<<0);//清除中断标志位
}
