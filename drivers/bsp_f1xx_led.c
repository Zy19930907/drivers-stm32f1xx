#include "bsp_f1xx_led.h"

void ledinit(_led *led)
{
	bsp_IoInit(&led->io);
}

void ledoff(_led *led)
{
	if (led->lightlevel)
		bsp_IoClr(&led->io);
	else
		bsp_IoSet(&led->io);
}

void ledon(_led *led)
{
	if (led->lightlevel)
		bsp_IoSet(&led->io);
	else
		bsp_IoClr(&led->io);
}

void ledfilp(_led *led)
{
	led->io.port->ODR ^= 1 << led->io.pinx;
}
