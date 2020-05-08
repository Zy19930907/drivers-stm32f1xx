//
// Created by 周洋 on 2020/5/8.
//

#include "temp_ds18b20.h"

static inline void ds18b20_delay_us(u32 nus)
{
	mod_delay_us(nus);
}

static inline void ds18b20_delay_ms(u32 nms)
{
	mod_delay_ms(nms);
}

static inline void SwitchDqDir(_ds18b20 *ds18B20, u8 dir)
{
	mod_IoSetDir(&ds18B20->dq, dir);
}

static inline void SwitchDqOut(_ds18b20 *ds18B20, u8 val)
{
	mod_IoSetVal(&ds18B20->dq, val);
}

static inline u8 GetDq(_ds18b20 *ds18B20)
{
	return mod_IoGetVal(&ds18B20->dq);
}

static inline void Ds18b20Reset(_ds18b20 *ds18B20)
{
	SwitchDqDir(1);   //SET PG11 OUTPUT
	SwitchDqOut(ds18B20, 0);
	ds18b20_delay_us(750);      //拉低750us
	SwitchDqOut(ds18B20, 1);
	ds18b20_delay_us(15);       //15US
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
static inline u8 DS18B20_Check(_ds18b20 *ds18B20)
{
	u8 retry = 0;
	SwitchDqDir(0);  //SET PG11 INPUT
	while (GetDq(ds18B20) && retry < 200) {
		retry++;
		ds18b20_delay_us(1);
	};
	if (retry >= 200)
		return 1;
	else
		retry = 0;
	while (!GetDq(ds18B20) && retry < 240) {
		retry++;
		ds18b20_delay_us(1);
	};
	if (retry >= 240)
		return 1;
	return 0;
}

static inline u8 DS18B20_Read_Bit(_ds18b20 *ds18B20)
{
	u8 data;
	SwitchDqDir(ds18B20, 1);   //SET PG11 OUTPUT
	SwitchDqOut(ds18B20, 0);
	ds18b20_delay_us(2);
	SwitchDqOut(ds18B20, 1);
	SwitchDqDir(ds18B20, 0);; //SET PG11 INPUT
	ds18b20_delay_us(12);
	if (GetDq(ds18B20))
		data = 1;
	else
		data = 0;
	ds18b20_delay_us(50);
	return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
static inline u8 DS18B20_Read_Byte(_ds18b20 *ds18B20)
{
	u8 i, j, dat;
	dat = 0;
	for (i = 1; i <= 8; i++) {
		j = DS18B20_Read_Bit(ds18B20);
		dat = (j << 7) | (dat >> 1);
	}
	return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
static inline void DS18B20_Write_Byte(_ds18b20 *ds18B20, u8 dat)
{
	u8 j;
	u8 testb;
	SwitchDqDir(ds18B20, 1);   //SET PG11 OUTPUT
	for (j = 1; j <= 8; j++) {
		testb = dat & 0x01;
		dat = dat >> 1;
		if (testb) {
			SwitchDqOut(ds18B20, 0);
			ds18b20_delay_us(2);
			SwitchDqOut(ds18B20, 1);
			delay_us(60);
		} else {
			SwitchDqOut(ds18B20, 0);
			ds18b20_delay_us(60);
			SwitchDqOut(ds18B20, 1);
			ds18b20_delay_us(2);
		}
	}
}

//开始温度转换
static inline void DS18B20_Start(_ds18b20 *ds18B20)
{
	Ds18b20Reset(ds18B20);
	DS18B20_Check(ds18B20);
	DS18B20_Write_Byte(0xcc); // skip rom
	DS18B20_Write_Byte(0x44); // convert
}

u8 mod_Ds18b20Init(_ds18b20 *ds18B20)
{
	mod_IoInit(&ds18B20->dq);
	Ds18b20Reset(ds18B20);
	return DS18B20_Check(ds18B20);
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250）
void mod_Ds18b20GetTemo(_ds18b20 *ds18B20)
{
	u8 temp;
	u8 TL, TH;
	short tem;
	DS18B20_Start(ds18B20); // ds1820 start convert
	Ds18b20Reset(ds18B20);
	DS18B20_Check(ds18B20);
	DS18B20_Write_Byte(ds18B20,0xcc); // skip rom
	DS18B20_Write_Byte(ds18B20,0xbe); // convert
	TL = DS18B20_Read_Byte(ds18B20); // LSB
	TH = DS18B20_Read_Byte(ds18B20); // MSB

	if (TH > 7)
	{
		TH = ~TH;
		TL = ~TL;
		temp = 0; //温度为负
	}
	else
		temp = 1; //温度为正
	tem = TH;     //获得高八位
	tem <<= 8;
	tem += TL;                //获得底八位
	tem = (float)tem * 0.625; //转换
	if (temp)
		ds18B20->temp = tem;
	else
		ds18B20->temp = -tem;
}
