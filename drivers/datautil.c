//
// Created by 周洋 on 2020/3/31.
//

#include "datautil.h"
#include "bsp_f1xx_uart.h"

_DataUtil* CreateDataUtil(_DataUtilType type,void *args)
{
    _DataUtil *util = 0;
    switch(type)
    {
        case DataUtil_UART:
            util = bsp_UartInit((_uartpara*)args);
            break;

        case DataUtil_SPI:
            break;
    }

    return util;
}


void CloseDataUtil(_DataUtilType type,_DataUtil *util)
{
	switch(type)
	{
		case DataUtil_UART:
			bsp_UartClose(util);
			myfree(util);
			break;

		case DataUtil_SPI:
			break;
	}
}
