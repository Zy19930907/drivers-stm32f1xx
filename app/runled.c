#include "runled.h"
#include "tskmanager.h"
#include "bsp_f1xx_led.h"


void runledfunc(void *args);

_led runled = {
		.io = {PB, 12, IO_MODE_OUT_PP_50MHZ,},
		.lightlevel = 0,
};

_Task runledtsk = {
		.ms = 100,
		.func = runledfunc,
};

void runledfunc(void *args)
{
	ledfilp(&runled);
}

void runledinit(void)
{
	ledinit(&runled);
	CreateTask(&runledtsk);
}

void SetRunledTime(u32 ms)
{
	SetTaskPeriod(0,0,0,0,ms,&runledtsk);
}
