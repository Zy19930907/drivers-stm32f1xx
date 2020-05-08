#include "bsp_soft_i2c.h"

static inline void i2c_delay_us(u32 nus)
{
	delay_us(nus);
}

static inline void SDA_Switch(_I2C *i2c, u8 val)
{
	if (val)
		bsp_IoSet(&i2c->sda);
	else
		bsp_IoClr(&i2c->sda);
}

static inline void SCL_Switch(_I2C *i2c, u8 val)
{
	if (val)
		bsp_IoSet(&i2c->scl);
	else
		bsp_IoClr(&i2c->scl);
}

static inline u8 I2CRead(_I2C *i2c)
{
	return bsp_IoRead(&i2c->sda);
}

static inline void SwitchDir(_I2C *i2c, u8 dir)
{
	if (dir)//输出
		i2c->sda.mode = IO_MODE_IN_PULL;
	else
		i2c->sda.mode = IO_MODE_OUT_PP_50MHZ;
	bsp_IoInit(&i2c->sda);
}

void bsp_SoftI2CInit(_I2C *i2c)
{
	bsp_IoInit(&i2c->sda);
	bsp_IoInit(&i2c->scl);
	SDA_Switch(i2c,1);
	SCL_Switch(i2c,1);
}


static inline void bsp_SoftI2CStart(_I2C *i2c)
{
	SwitchDir(i2c, 1);//sda线输出
	SDA_Switch(i2c, 1);
	SCL_Switch(i2c, 1);
	i2c_delay_us(2);
	SDA_Switch(i2c, 0);
	i2c_delay_us(2);
	SCL_Switch(i2c, 0);
}

static inline void bsp_SoftI2CStop(_I2C *i2c)
{
	SwitchDir(i2c, 1);//sda线输出
	SCL_Switch(i2c, 0);
	SDA_Switch(i2c, 0);
	i2c_delay_us(2);
	SDA_Switch(i2c, 1);
	SCL_Switch(i2c, 1);
	i2c_delay_us(2);
}

static inline u8 bsp_SoftI2CWaitAck(_I2C *i2c)
{
	u8 ucErrTime = 0;
	SwitchDir(i2c, 0);      //SDA设置为输入
	SDA_Switch(i2c,1);
	i2c_delay_us(2);
	SCL_Switch(i2c,1);
	i2c_delay_us(2);

	while (I2CRead(i2c)) {
		ucErrTime++;
		if (ucErrTime > 250) {
			bsp_SoftI2CStop(i2c);
			return 1;
		}
	}
	SCL_Switch(i2c,0);
	return 0;
}

static inline void bsp_SoftI2CAck(_I2C *i2c)
{
	SCL_Switch(i2c,0);
	SwitchDir(i2c, 1);
	SDA_Switch(i2c, 0);
	i2c_delay_us(2);
	SCL_Switch(i2c, 1);
	i2c_delay_us(2);
	SCL_Switch(i2c,0);
}

static inline void bsp_SoftI2cNack(_I2C *i2c)
{
	SCL_Switch(i2c,0);
	SwitchDir(i2c, 1);
	SDA_Switch(i2c,1);
	i2c_delay_us(2);
	SCL_Switch(i2c, 1);
	i2c_delay_us(2);
	SCL_Switch(i2c,0);
}

static inline void bsp_SoftI2CSendByte(_I2C *i2c, u8 dat)
{
	u8 t;
	SwitchDir(i2c, 1);
	SCL_Switch(i2c,0);
	for (t = 0x80; t > 0; t >>= 1) {
		if (dat & t)
			SDA_Switch(i2c,1);
		else
			SDA_Switch(i2c, 0);
		SCL_Switch(i2c, 1);
		i2c_delay_us(2);
		SCL_Switch(i2c,0);
		i2c_delay_us(2);
	}
}

static inline u8 bsp_SoftI2CReadByte(_I2C *i2c, u8 ack)
{
	unsigned char i, receive = 0;
	SwitchDir(i2c, 0);//SDA设置为输入
	for (i = 0; i < 8; i++) {
		SCL_Switch(i2c,0);
		i2c_delay_us(2);
		SCL_Switch(i2c, 1);
		receive <<= 1;
		if (bsp_IoRead(&i2c->sda))receive++;
		i2c_delay_us(2);
	}
	if (!ack)
		bsp_SoftI2CAck(i2c);//发送nACK
	else
		bsp_SoftI2cNack(i2c); //发送ACK
	return receive;
}

s16 bsp_SoftI2CWrite(_I2C *i2c, u8 addr, u8 reg, u8 *dat, u16 len)
{
	u16 i = 0;
	bsp_SoftI2CStart(i2c);
	bsp_SoftI2CSendByte(i2c, (addr << 1) | 0);//发送器件地址+写命令
	if (bsp_SoftI2CWaitAck(i2c))    //等待应答
	{
		bsp_SoftI2CStop(i2c);
		return -1;
	}
	bsp_SoftI2CSendByte(i2c, reg);    //写寄存器地址
	bsp_SoftI2CWaitAck(i2c);        //等待应答
	for (i = 0; i < len; i++) {
		bsp_SoftI2CSendByte(i2c, dat[i]);    //发送数据
		if (bsp_SoftI2CWaitAck(i2c))        //等待ACK
		{
			break;
		}
	}
	bsp_SoftI2CStop(i2c);
	return i;
}

s16 bsp_SoftI2CRead(_I2C *i2c, u8 addr, u8 reg, u8 *dat, u16 len)
{
	bsp_SoftI2CStart(i2c);
	bsp_SoftI2CSendByte(i2c, (addr << 1) | 0);//发送器件地址+写命令
	if (bsp_SoftI2CWaitAck(i2c))    //等待应答
	{
		bsp_SoftI2CStop(i2c);
		return -1;
	}
	bsp_SoftI2CSendByte(i2c, reg);    //写寄存器地址
	bsp_SoftI2CWaitAck(i2c);        //等待应答

	bsp_SoftI2CStart(i2c);
	bsp_SoftI2CSendByte(i2c, (addr << 1) | 1);//发送器件地址+读命令
	bsp_SoftI2CWaitAck(i2c);        //等待应答
	while (len) {
		if (len == 1)*dat = bsp_SoftI2CReadByte(i2c, 0);//读数据,发送nACK
		else *dat = bsp_SoftI2CReadByte(i2c, 1);        //读数据,发送ACK
		len--;
		dat++;
	}
	bsp_SoftI2CStop(i2c);    //产生一个停止条件
	return 0;
}

s16 bsp_SoftI2CWriteReg(_I2C *i2c, u8 addr, u8 reg, u8 dat)
{
	bsp_SoftI2CStart(i2c);
	bsp_SoftI2CSendByte(i2c, (addr << 1) | 0);//发送器件地址+写命令
	if (bsp_SoftI2CWaitAck(i2c))    //等待应答
	{
		bsp_SoftI2CStop(i2c);
		return -1;
	}
	bsp_SoftI2CSendByte(i2c, reg);    //写寄存器地址
	bsp_SoftI2CWaitAck(i2c);        //等待应答

	bsp_SoftI2CSendByte(i2c, dat);//发送数据
	if (bsp_SoftI2CWaitAck(i2c))    //等待ACK
	{
		bsp_SoftI2CStop(i2c);
		return -1;
	}
	bsp_SoftI2CStop(i2c);
	return 0;
}

u8 bsp_SoftI2CReadReg(_I2C *i2c, u8 addr, u8 reg)
{
	u8 res;
	bsp_SoftI2CStart(i2c);
	bsp_SoftI2CSendByte(i2c, (addr << 1) | 0);//发送器件地址+写命令
	bsp_SoftI2CWaitAck(i2c);
	bsp_SoftI2CSendByte(i2c, reg);    //写寄存器地址
	bsp_SoftI2CWaitAck(i2c);        //等待应答
	bsp_SoftI2CStart(i2c);
	bsp_SoftI2CSendByte(i2c, (addr << 1) | 1);//发送器件地址+读命令
	bsp_SoftI2CWaitAck(i2c);    //等待应答
	res = bsp_SoftI2CReadByte(i2c, 0);//读取数据,发送nACK
	bsp_SoftI2CStop(i2c);            //产生一个停止条件
	return res;
}
