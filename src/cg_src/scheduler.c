/*
 * scheduler.c
 *
 *  Created on: 2017年8月3日
 *      Author: LWD
 */

#include "scheduler.h"
#include "Navi.h"
#include "data_transfer.h"
#include "r_cg_sci.h"

loop_t loop;

/**************************************************************************************************************
*
*	给Cop发送：void Data_Transmit1_Cop(uint8_t *data_buff);	//传入被发10字节数据帧的头地址
*	给Gnd发送：void Data_Transmit5_Gnd(uint8_t *data_buff);	//传入被发10字节数据帧的头地址
*
/*************************************************************************************************************/
Duty_50ms()
{
	Navigate();		//执行导航任务
}

Duty_100ms()		//数传+串口的任务
{

	static int cnt,cmt;
	cmt=f_info.Gnd_ctrl.act;
	if(!(cmt==0))
	cnt++;


	if(f_info.ctrl_mode==1)			//Ground手动控制
	{
		data_trans(f_info.Gnd_ctrl.act,f_info.Gnd_ctrl.deg);
		if(cnt>=cmt)
		{
			cnt=0;
			PORT7.PODR.BIT.B6 = ~PORT7.PODR.BIT.B6;
		}
	}
	else if(f_info.ctrl_mode==2)	//队列自动控制
	{
		data_trans(f_info.Auto_ctrl.act,f_info.Auto_ctrl.deg);
	}

}

Duty_2s()
{
	//data_trans(f_info.Gnd_ctrl.act,f_info.Gnd_ctrl.deg);
}

void Loop_check(void)
{
	loop.time++;
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	loop.cnt_100ms++;
	loop.cnt_1s++;
	loop.cnt_2s++;
	if( loop.check_flag==1 )
	{loop.err_flag ++;}
	else
	{loop.check_flag = 1;}
}
void Duty_Loop()   					//最短任务周期为1ms，总的代码执行时间需要小于1ms。
{
	if( loop.check_flag == 1 )
	{

			if( loop.cnt_50ms >= 50 )
			{
				loop.cnt_50ms = 0;
				Duty_50ms();					//周期50ms的任务
			}
			if( loop.cnt_100ms >= 100)
			{
				loop.cnt_100ms = 0;
				Duty_100ms();
			}
			if( loop.cnt_2s >= 2000)
			{
				loop.cnt_2s = 0;
				Duty_2s();
			}
			loop.check_flag = 0;
	}
}
