#include "bsp_f1xx_flash.h"

/*
===========================================================================================
* 功    能：Flash页擦除
* 输    入：page：需要擦除的Flash页序号
* 输    出：无
===========================================================================================
 */
void bsp_EraseFlash(u16 page)
{
	UNLOCKFLASH;
	FLASH->CR |= EARSEFLASH_PAGE;
	while (FLASH->SR & 0x01); //等待falsh空闲
	FLASH->AR = (FLASHSTARTADDR + (page * FLASHPAGESIZE));
	FLASH->CR |= FLASHEARSESTART;
	while (FLASH->SR & 0x01); //等待falsh空闲

	FLASH->CR &= ~EARSEFLASH_PAGE;
	LOCKFALSH;
}

void bsp_WriteFlash(u32 wAddr, u8 *dat, u16 len)
{
	u16 *p, *q;
	len >>= 1;

	p = (u16 *) wAddr;
	q = (u16 *) dat;

	UNLOCKFLASH;
	FLASH->CR |= FLASH_PROGRAM;
	while (FLASH->SR & 0x01); //等待falsh空闲
	do {
		*p++ = *q++;
		while (FLASH->SR & 0x01); //等待falsh空闲
	} while (--len);
	FLASH->CR &= ~FLASH_PROGRAM; //退出FLASH编程模式
	LOCKFALSH;
}

void bsp_ReadFlash(u32 rAddr, u8 *dat, u16 len)
{
	len >>= 1;
	u16 *p = (u16 *) dat;
	u16 *q = (u16 *) rAddr;
	do {
		*p++ = *q++;
	} while (--len);
}