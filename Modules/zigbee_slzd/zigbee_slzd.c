//
// Created by 周洋 on 2020/5/8.
//

#include "zigbee_slzd.h"

void mod_SlzdZgbInit(_zgb_slzd *zgb)
{
	zgb->util = CreateDataUtil(DataUtil_UART, &zgb->para);

}

static inline void ResetPack(_zgb_slzd *zgb)
{
	zgb->recvLen = 0;
	zgb->headRecved = 0;
	zgb->needNextByte = 0;
}

void OnZigbeeDataIn(_zgb_slzd *zgb, u8 *dat, u16 len)
{
	u8 i = 0,j=0;
	if (!zgb->headRecved) {
		for (i = 0; i < len; i++) {
			if (*(dat + i) == 0xFE) {
				zgb->headRecved = 1;
				i++;
				break;
			}
		}
	}

	if (zgb->headRecved) {
		for (; i < len; i++) {

			if (zgb->recvLen >= MAXZIGBEEPACKLEN) {
				ResetPack(zgb);
				i++;
				break;
			}

			switch (*(dat + i)) {
				case 0xFC:
					if (zgb->needNextByte) {
						zgb->pack[zgb->recvLen++] = 0xFE;
						zgb->needNextByte = 0;
					} else
						zgb->pack[zgb->recvLen++] = 0xFC;
					break;
				case 0xFD:
					if (zgb->needNextByte) {
						zgb->pack[zgb->recvLen++] = 0xFF;
						zgb->needNextByte = 0;
					} else
						zgb->pack[zgb->recvLen++] = 0xFD;
					break;
				case 0xFE:
					zgb->needNextByte = 1;
					break;
				case 0xFF:
					zgb->packRecver(zgb->pack[2], zgb->pack[3], *(u16 *) &zgb->pack[4], &zgb->pack[6], zgb->pack[1] - 4);
					ResetPack(zgb);
					goto RamainDataDeal;
				default:
					zgb->pack[zgb->recvLen++] = dat[i];
					break;
			}
		}
	}

	RamainDataDeal:
		for(;i<len;i++)
		{
			zgb->temp[j++] = dat[i];
			OnZigbeeDataIn(zgb,zgb->temp,j);
		}
}

void mod_ZgbSlzdSendPack(_zgb_slzd *zgb)
{
	u8 pack[MAXZIGBEEPACKLEN];
	u8 i = 0, j = 0;
	pack[i++] = 0xFE;//起始符
	pack[i++] = zgb->msgLen + 4;
	pack[i++] = zgb->srcPort;
	pack[i++] = zgb->desPort;
	*(u16 *) &pack[i] = zgb->desAddr;
	i += 2;
	for (; j < zgb->msgLen; j++) {
		switch (zgb->msg[j]) {
			case 0xFF://当发送的数据为0xFF时转义
				pack[i++] = 0xFE;
				pack[i++] = 0xFD;
				break;
			case 0xFE://当发送的数据为0xFE时转义
				pack[i++] = zgb->msg[j];
				pack[i++] = 0xFC;
				break;
			default:
				pack[i++] = zgb->msg[j];
				break;
		}
	}
	pack[i++] = 0xFF;//结束符
	zgb->util->write(pack,i);
}

void mod_ZgbReset(_zgb_slzd *zgb)
{
	zgb->srcPort = 0x90;
	zgb->desPort = 0x21;
	zgb->desAddr = 0;
	zgb->msgLen = 1;
	zgb->msg[0] = 0x10;
	mod_ZgbSlzdSendPack(zgb);
	mod_delay_ms(100);
}

void mod_ZgbSetAddr(_zgb_slzd *zgb,u16 addr)
{
	zgb->srcPort = 0x90;
	zgb->desPort = 0x21;
	zgb->desAddr = 0;
	zgb->msgLen = 2;
	*(u16*)zgb->msg = addr;
	mod_ZgbSlzdSendPack(zgb);
	mod_delay_ms(100);
	mod_ZgbReset(zgb);
}
