//
// Created by 周洋 on 2020/4/7.
//

#ifndef RECORDERCONFIGER_BSP_F1XX_RTC_H
#define RECORDERCONFIGER_BSP_F1XX_RTC_H
#include "public.h"

//时间结构体
typedef struct
{
	u8 hour;
	u8 min;
	u8 sec;
	//公历日月年周
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;
	u32 seCnt;
}_MTIME;
extern _MTIME mTime;	//日历结构体

void Disp_Time(u8 x,u8 y,u8 size);			//在制定位置开始显示时间
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);	//在指定位置显示星期
u8 bsp_RTC_Init(void);        		//初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);		//平年,闰年判断
u8 RTC_Get(void);         		//获取时间
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间
void RtcSetBySec(u32 sec);
#endif //RECORDERCONFIGER_BSP_F1XX_RTC_H
