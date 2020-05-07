//
// Created by 周洋 on 2020/4/7.
//

#ifndef RECORDERCONFIGER_BUFFERUTIL_H
#define RECORDERCONFIGER_BUFFERUTIL_H

#include "public.h"

typedef struct {
	volatile u64 tick;
	s32 r;
	s32 w;
	s32 size;
	u8 *buf;
}_Buffer;

extern void InitBuffer(_Buffer *buffer);
extern s32 GetPopBufferLen(_Buffer *buffer);
extern void PopBuffer(_Buffer *buffer,u8 *dat,s32 len);
extern void PushBuffer(_Buffer *buffer,u8 *dat,u16 len);
extern void BufferReset(_Buffer *buffer);

#endif //RECORDERCONFIGER_BUFFERUTIL_H
