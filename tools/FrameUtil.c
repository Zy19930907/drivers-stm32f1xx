//
// Created by 周洋 on 2020/4/23.
//

#include "FrameUtil.h"
#include "CrcUtil.h"

typedef void OnFrameRecv(u8 *buf,u8 len);
OnFrameRecv *OnFrameRecved;

static u32 frameHead = 0x55333355;
static u8 frameDat[512];
static u8 temp[512];
static u8 frame[512];
static int frameLen = 0;
static u16 recvLen = 0;
static u8 headRecved = 0;

void FrameReset(void)
{
	headRecved = 0;
	frameLen = 0;
	recvLen = 0;
}

void OnFrameDataIn(u8 *dat, u16 len)
{
	u16 i=0;
	if (!headRecved)
	{
		for (; i < len; i++)
		{
			frameDat[recvLen++] = dat[i];
			if (recvLen == 4) {
				if (frameDat[0] != ((frameHead >> 24) & 0xFF))
				{
					frameDat[0] = frameDat[1];
					frameDat[1] = frameDat[2];
					frameDat[2] = frameDat[3];
					recvLen = 3;
				}
				else if (frameDat[1] != ((frameHead >> 16) & 0xFF))
				{
					frameDat[0] = frameDat[2];
					frameDat[1] = frameDat[3];
					recvLen = 2;
				}
				else if (frameDat[2] != ((frameHead >> 8) & 0xFF))
				{
					frameDat[0] = frameDat[3];
					recvLen = 1;
				}
				else if (frameDat[3] != (frameHead & 0xFF))
				{
					recvLen = 0;
				}
				else if ((frameDat[0] == (frameHead >> 24 & 0xFF)) && (frameDat[1] == (frameHead >> 16 & 0xFF)) && (frameDat[2] == (frameHead >> 8 & 0xFF)) && (frameDat[3] == (frameHead >> 0 & 0xFF)))
				{
					headRecved = 1;
					i++;
					break;
				}
			}
		}
	}

	if (headRecved) {
		for (; i < len; i++) {
			frameDat[recvLen++] = dat[i];
			if (recvLen >= 6)
				frameLen = *(u16*)&frameDat[4];

			if (recvLen == frameLen)
			{
				if (CrcUtil.CheckCrc(frameDat, frameLen))
				{
					memcpy(frame,&frameDat[6],frameLen-8);
					OnFrameRecved(frame,frameLen-8);
				}
				FrameReset();
				i++;
				break;
			}
		}

		if (i < len)
		{
			memcpy(frameDat,&dat[i],len-i);
			OnFrameDataIn(temp, len-i);
		}
	}
}

u16 MakeFrame(u8 *msg,u16 len)
{
	u8 out[512];
	*(u32*)out = 0x55333355;
	*(u16*)&out[4] = (len+8);
	memcpy(&out[6],msg,len);
	CrcUtil.SetCrc(out,len+8);
	memcpy(msg,out,len+8);
	return len+8;
}

void SetFrameRecver(void *recver(u8 *buf,u8 len))
{
	OnFrameRecved = recver;
}
