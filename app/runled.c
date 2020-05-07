//
// Created by 周洋 on 2020/3/29.
//

#include "runled.h"
#include "tskmanager.h"
#include "bsp_f1xx_led.h"


void runledfunc(void *args);
_led runled = {
        .io = {PB,12,IO_MODE_OUT_PP_50MHZ,},
        .lightlevel = 0,
};

_Task runledtsk={
        .ms = 30,
        .func = runledfunc,
};

void runledfunc(void *args)
{
	ledinit(&runled);
    ledfilp(&runled);
}

void runledinit(void)
{
    CreateTask(&runledtsk);
}

void SetRunledTime(u32 ms)
{
	runledtsk.period = ms;
}
