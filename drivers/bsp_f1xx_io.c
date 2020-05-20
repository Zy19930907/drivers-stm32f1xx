#include "bsp_f1xx_io.h"

void bsp_IoInit(_io *io)
{
	u8 offset = 2 + ((((u32) io->port) - GPIOA_BASE) / 0x400);
	MyGPIO_TypeDef *port = io->port;

	RCC->APB2ENR |= 1 << offset;
	port->CR &= ~(0x0F << (io->pinx << 2));
	port->CR |= ((u64) io->mode) << (io->pinx << 2);
}

void bsp_IoSet(_io *io)
{
	io->port->BSRR = 1 << io->pinx;
}

void bsp_IoClr(_io *io)
{
	io->port->BRR = 1 << io->pinx;
}

u8 bsp_IoRead(_io *io)
{
	return ((io->port->IDR >> io->pinx) & 0x01);
}
