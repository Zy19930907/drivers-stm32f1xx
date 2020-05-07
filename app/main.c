//
// Created by 周洋 on 2020/3/29.
//
#include "public.h"
#include "bsp_f1xx_timer.h"
#include "bsp_f1xx_uart.h"
#include "runled.h"
#include "mod_mpu6050.h"

_timer tiktimer = {
		.timx = TIM2,
		.sec = 0,
		.ms = 1,
		.ontimout = TskTickHandler,
};

void OnMsgRecv(u8 *msg,u16 len)
{

}

_DataUtil *util;
_uartpara uartpara={
		.comx = COM2,
		.baud = 115200,
		.rxbuflen = 1024,
		.txbuflen = 1024,
		.recver = OnMsgRecv,
};

float test = 234.123;
u8 *msg;

void UploadTsk(void *args)
{
	sprintf((char*)msg,"test:%.3f\r\n",test);
	util->write(msg,strlen((char*)msg));
}

_Task uploadTsk = {
		.ms = 1000,
		.func = UploadTsk,
};

int main(void)
{
	Stm32_Clock_Init(9);
	delay_init(72);
	my_mem_init();
	bsp_TimerInit(&tiktimer);

	runledinit();
	msg = mymalloc(100);
	util = CreateDataUtil(DataUtil_UART,&uartpara);
	CreateTask(&uploadTsk);
	mod_Mpu6050Init();

	ExecTask();
}
