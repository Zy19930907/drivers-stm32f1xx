//
// Created by 周洋 on 2020/4/7.
//

#include "bufferutil.h"
void InitBuffer(_Buffer *buffer)
{
	buffer->r = 0;
	buffer->w = 0;
	buffer->buf = mymalloc(buffer->size);
}

void BufferReset(_Buffer *buffer)
{
	buffer->r = 0;
	buffer->w = 0;
	memset(buffer->buf,'\0',buffer->size);
}

s32 GetPopBufferLen(_Buffer *buffer)
{
	s32 temp=0;
	temp = buffer->w - buffer->r;
	return (temp>=0 ? temp : temp+buffer->size);
}

void PushBuffer(_Buffer *buffer,u8 *dat,u16 len)
{
	if(len <=0)
		return;
	do
	{
		buffer->buf[buffer->w++] = *dat++;
		if(buffer->w>=buffer->size)
			buffer->w = 0;
	}while (--len);
	buffer->tick = TaskManger.tick;
}

void PopBuffer(_Buffer *buffer,u8 *dat,s32 len)
{
	if(len <= 0)
		return;
	do
	{
		*dat++ = buffer->buf[buffer->r++];
		if(buffer->r >= buffer->size)
			buffer->r = 0;
	}while (--len);
}
