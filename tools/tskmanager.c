#include "tskmanager.h"
#include "malloc.h"

static inline void StartTask(void *args);
static inline void EndTask(void *args);
u64 TaskTickDiff(u64 tick);
static inline void DelayMs(u64 nms);

extern _Task eTask;
_Task sTask = {
	.day = 0,
	.hour = 0,
	.min = 0,
	.sec = 0,
	.ms = 0,
	.func = StartTask,
	.pre = &eTask,
	.next = &eTask,
};

_Task eTask = {
	.day = 0,
	.hour = 0,
	.min = 0,
	.sec = 0,
	.ms = 0,
	.func = EndTask,
	.pre = &sTask,
	.next = &sTask,
};

_TaskManger TaskManger = {0, 0, &sTask, &sTask, &eTask, TaskTickDiff, DelayMs};

//计算任务执行时钟
u64 TaskTickDiff(u64 tick)
{
	if (tick > TaskManger.tick)
		return 0xFFFFFFFFFFFFFFFF - tick + TaskManger.tick;
	else
		return TaskManger.tick - tick;
}

//默认创建任务，位于任务链表开始节点
static inline void StartTask(void *args)
{

}

//默认创建任务，位于任务链表结束节点
static inline void EndTask(void *args)
{

}

//创建新任务，添加到任务链表结束节点的上一节点
void CreateTask(_Task *tsk)
{
	tsk->tick = TaskManger.tick;
	tsk->period = (((u64)tsk->day * 86400000) + ((u64)tsk->hour * 3600000) + ((u64)tsk->min * 60000) + ((u64)tsk->sec * 1000) + (u64)tsk->ms);        //计算任务执行周期
	tsk->pre = TaskManger.lasTask->pre;
	tsk->next = TaskManger.lasTask;
	TaskManger.lasTask->pre->next = tsk;
	TaskManger.lasTask->pre = tsk;
}

//从任务链表中删除任务
void DelTask(_Task *delTask)
{
	if (delTask != 0)
	{
		delTask->pre->next = delTask->next;
		delTask->next->pre = delTask->pre;
		myfree(delTask);
	}
}

//修改任务执行周期
void SetTaskPeriod(u64 day, u64 hour, u64 min, u64 sec, u64 ms, _Task *Task)
{
	Task->period = ((day * 86400000) + (hour * 3600000) + (min * 60000) + (sec * 1000) + ms);        //计算任务执行周期
}

volatile u64 delayTick;
void DelayMs(u64 nms)
{
	delayTick = TaskManger.tick;
	while (TaskTickDiff(delayTick) < nms);
}

void TskTickHandler(void)
{
    TaskManger.tick++;
}

_Noreturn //循环执行任务链表中的任务
void ExecTask(void)
{
	_Task *RunTask = &sTask;
	for (;;)
	{
		if (TaskTickDiff(RunTask->tick) >= RunTask->period)
		{
			RunTask->tick = TaskManger.tick;
			RunTask->func(RunTask->args);//调用任务处理函数
		}
		RunTask = RunTask->next;
	}
}

