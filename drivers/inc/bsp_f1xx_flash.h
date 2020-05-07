//
// Created by 周洋 on 2020/4/9.
//

#ifndef RECORDERCONFIGER_BSP_F1XX_FLASH_H
#define RECORDERCONFIGER_BSP_F1XX_FLASH_H

#include "public.h"

#define FLASHPAGESIZE        2048

#define EARSEFLASH_PAGE 0x02 //页擦除模式
#define FLASH_PROGRAM 0x01   //Flash编程模式
#define FLASHEARSESTART 0x40 //开始擦除

#define LOCKFALSH 		FLASH->CR = BIT7 //锁Flash
#define UNLOCKFLASH		{FLASH->KEYR = 0x45670123;FLASH->KEYR = 0xCDEF89AB;}
#define FLASHSTARTADDR	0x08000000

void bsp_EraseFlash(u16 page);
void bsp_WriteFlash(u32 wAddr,u8 *dat,u16 len);
void bsp_ReadFlash(u32 rAddr,u8 *dat,u16 len);

#endif //RECORDERCONFIGER_BSP_F1XX_FLASH_H
