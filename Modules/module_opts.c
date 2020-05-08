//
// Created by 周洋 on 2020/5/8.
//

#include "module_opts.h"

void mod_delay_us(u32 nus)
{
	delay_us(nus);
}

void mod_delay_ms(u32 nms)
{
	delay_ms(nms);
}

void mod_IoInit(_io *io)
{
	bsp_IoInit(io);
}

void mod_IoSetDir(_io *io,u8 dir)
{
	if(dir)
		io->mode = IO_MODE_OUT_PP_50MHZ;
	else
		io->mode = IO_MODE_IN_PULL;
	bsp_IoInit(io);
}

void mod_IoSetVal(_io *io,u8 val)
{
	if(val)
		bsp_IoSet(io);
	else
		bsp_IoClr(io);
}

u8 mod_IoGetVal(_io *io)
{
	return bsp_IoRead(io);
}
