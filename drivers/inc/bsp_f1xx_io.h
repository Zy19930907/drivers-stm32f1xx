//
// Created by 周洋 on 2020/3/29.
//

#ifndef __BSP_F1XX_IO_H
#define __BSP_F1XX_IO_H

#include "public.h"

typedef enum
{
    IO_MODE_IN_AF = 0,
    IO_MODE_IN_FLOAT = 4,
    IO_MODE_IN_PULL = 8,
    IO_MODE_OUT_PP_2MHZ = 1,
    IO_MODE_OUT_PP_10MHZ = 2,
    IO_MODE_OUT_PP_50MHZ = 3,
    IO_MODE_OUT_OD_2MHZ = 5,
    IO_MODE_OUT_OD_10MHZ = 6,
    IO_MODE_OUT_OD_50MHZ = 7,
    IO_MODE_OUT_AF_PP_2MHZ = 9,
    IO_MODE_OUT_AF_PP_10MHZ = 10,
    IO_MODE_OUT_AF_PP_50MHZ = 11,
    IO_MODE_OUT_AF_OD_2MHZ = 13,
    IO_MODE_OUT_AF_OD_10MHZ = 14,
    IO_MODE_OUT_AF_OD_50MHZ = 15,
} IO_MODE;

typedef struct
{
    __IO uint64_t CR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t BRR;
    __IO uint32_t LCKR;
} MyGPIO_TypeDef;

#define PA                    ((MyGPIO_TypeDef *)GPIOA_BASE)
#define PB                    ((MyGPIO_TypeDef *)GPIOB_BASE)
#define PC                    ((MyGPIO_TypeDef *)GPIOC_BASE)
#define PD                    ((MyGPIO_TypeDef *)GPIOD_BASE)
#define PE                    ((MyGPIO_TypeDef *)GPIOE_BASE)
#define PF                    ((MyGPIO_TypeDef *)GPIOF_BASE)
#define PG                    ((MyGPIO_TypeDef *)GPIOG_BASE)

typedef struct
{
    MyGPIO_TypeDef *port;
    u8 pinx;
    IO_MODE mode;
} _io;

void bsp_IoInit(_io *io);
void bsp_IoSet(_io *io);
void bsp_IoClr(_io *io);

#endif //STM32F1XX_BSP_F1XX_IO_H

