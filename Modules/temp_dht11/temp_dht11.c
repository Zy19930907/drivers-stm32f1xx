//
// Created by 周洋 on 2020/5/8.
//

#include "temp_dht11.h"

static inline void mod_Dht11Reset(_dht11 *dht11)
{
	mod_IoSetDir(&dht11->dq, 1);
	mod_IoSetVal(&dht11->dq, 0);
	mod_delay_ms(20);        //拉低至少18ms
	mod_IoSetVal(&dht11->dq, 1);
	mod_delay_us(30);        //主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
static inline u8 DHT11_Check(_dht11 *dht11)
{
	u8 retry = 0;
	mod_IoSetDir(&dht11->dq, 0);;//SET INPUT
	while (mod_IoGetVal(&dht11->dq) && retry < 100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	}
	if (retry >= 100)return 1;
	else retry = 0;
	while (!mod_IoGetVal(&dht11->dq) && retry < 100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	}
	if (retry >= 100)return 1;
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
static inline u8 DHT11_Read_Bit(_dht11 *dht11)
{
	u8 retry = 0;
	while (mod_IoGetVal(&dht11->dq) && retry < 100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry = 0;
	while (!mod_IoGetVal(&dht11->dq) && retry < 100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if (mod_IoGetVal(&dht11->dq))return 1;
	else return 0;
}

//从DHT11读取一个字节
//返回值：读到的数据
static inline u8 DHT11_Read_Byte(void)
{
	u8 i, dat;
	dat = 0;
	for (i = 0; i < 8; i++) {
		dat <<= 1;
		dat |= DHT11_Read_Bit();
	}
	return dat;
}

/*
===========================================================================================
* 功    能：
* 输    入：esp：wifi模块配置参数
* 输    出：无
===========================================================================================
 */
u8 mod_Dht11Read(_dht11 *dht11, u8 *temp, u8 *humi)
{
	u8 buf[5];
	u8 i;
	mod_Dht11Reset();
	if (DHT11_Check() == 0) {
		for (i = 0; i < 5; i++)//读取40位数据
		{
			buf[i] = DHT11_Read_Byte();
		}
		if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
			*humi = buf[0];
			*temp = buf[2];
		}
	} else return 1;
	return 0;
}

u8 mod_Dht11Init(_dht11 *dht11)
{
	mod_IoInit(&dht11->dq);
	mod_Dht11Reset(dht11);
	return DHT11_Check(dht11);
}
