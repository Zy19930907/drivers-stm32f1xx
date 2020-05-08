#include "public.h"
#include "bsp_f1xx_timer.h"
#include "runled.h"
#include "inv_mpu.h"

_timer tiktimer = {
		.timx = TIM2,
		.sec = 0,
		.ms = 1,
		.ontimout = TskTickHandler,
};

int main(void)
{
	Stm32_Clock_Init(9);
	delay_init(72);
	my_mem_init();
	bsp_TimerInit(&tiktimer);

	runledinit();

	ExecTask();
}
