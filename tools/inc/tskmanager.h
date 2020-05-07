#ifndef __TASKMANGER_H
#define __TASKMANGER_H

#include "public.h"

typedef struct _task
{
	u64 tick;			//任务执行时钟
	u64 period;		//任务执行间隔
	u64 day;
	u64 hour;
	u64 min;
	u64 sec;
	u64 ms;
	void *args;
	void(*func)(void *args);			//任务具体执行函数指针
	struct _task *pre;
	struct _task *next;
}_Task;

typedef struct
{
	volatile u64 tick;
	_Task *curTask;
	_Task *firTask;
	_Task *lasTask;
	u64(*TaskDiff)(u64 tick);
	void(*delay_ms)(u64 nms);
}_TaskManger;

extern _TaskManger TaskManger;

extern void CreateTask(_Task *tsk);
extern void SetTaskPeriod(u64 day, u64 hour, u64 min, u64 sec, u64 ms, _Task *Task);
extern void DelTask(_Task *delTask);

_Noreturn extern void ExecTask(void);
void TskTickHandler(void);

#endif
