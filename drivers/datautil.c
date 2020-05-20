#include "datautil.h"
#include "bsp_f1xx_uart.h"

_DataUtil *CreateDataUtil(_DataUtilType type, void *args)
{
	_DataUtil *util = 0;
	switch (type) {
		case DATAUTIL_UART:
			util = bsp_UartInit((_uartpara *) args);
			break;

		case DATAUTIL_SPI:
			break;

		case DATAUTIL_SOFTI2C:
			break;
		case DATAUTIL_SOFTSPI:
			break;
	}

	return util;
}


void CloseDataUtil(_DataUtilType type, _DataUtil *util)
{
	switch (type) {
		case DATAUTIL_UART:
			bsp_UartClose(util);
			myfree(util);
			break;

		case DATAUTIL_SPI:
			break;
		case DATAUTIL_SOFTI2C:
			break;
		case DATAUTIL_SOFTSPI:
			break;
	}
}
