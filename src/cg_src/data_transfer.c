/*
 * data_transfer.c
 *
 *  Created on: 2017年7月29日
 *      Author: LWD
 */

#include "data_transfer.h"
#include "r_cg_sci.h"
#include "Navi.h"
#include "r_cg_macrodriver.h"


unsigned char TXBUF[10];		//储存发送数据

/************************串口1接收**********************************************************************/
void Data_Receive1_Cop (uint8_t *data_buf)
{
	static unsigned char f_sta0/*,f_sta1,f_sta2*/;

	if( !((*data_buf==0xBB) && (*(data_buf+1)== 0xEE)) )
	{
		return;
	}
	if(*(data_buf+2)==0x00) 			//集合0：表示接收飞机目前飞行状态信息
	{
//		f_sta2 = f_sta1;
//		f_sta1 = f_sta0;

		if(*(data_buf+3)==0x00)			//异常
			f_sta0=0;
		else if(*(data_buf+3)==0x01)	//起飞+定高完成
			f_sta0=1;
		else if(*(data_buf+3)==0x02)	//前进完成
			f_sta0=2;
		else if(*(data_buf+3)==0x03)	//左转完成
			f_sta0=3;
		else if(*(data_buf+3)==0x04)	//右转完成
			f_sta0=4;
		else if(*(data_buf+3)==0x05)	//降高完成
			f_sta0=5;
		else if(*(data_buf+3)==0x06)	//升高完成
			f_sta0=6;
		else if(*(data_buf+3)==0x07)	//降落
			f_sta0=7;

	//	if((f_sta0==f_sta1)&&(f_sta1==f_sta2))
				f_info.status = f_sta0;			//更新飞行状态
	}
}

/***************串口1发送****************/

void data_trans(uint8_t act,uint8_t deg,uint8_t sci)
{
	TXBUF[0]=0xBB;
	TXBUF[1]=0xE1;
	TXBUF[2]=act;
	TXBUF[3]=deg;
	TXBUF[4]=TXBUF[5]=TXBUF[6]=TXBUF[7]=TXBUF[8]=TXBUF[9]=0;		//目前为0


	if(sci==1)
		Data_Transmit1_Cop(TXBUF);						//把任务包发出去
	else if(sci==5)
		Data_Transmit5_Gnd(TXBUF);
}
/***********串口1发送函数************************/
void Data_Transmit1_Cop (uint8_t *data_buff)
{
	SCI1_Send(data_buff);
}


/************************数传5接收解包**********************************************************************/
void Data_Receive5_Gnd (uint8_t *data_buf)
{
	if( !((*data_buf==0xAA) && (*(data_buf+1)== 0xEE)) )
	{
		return;
	}
	if(*(data_buf+2)==0x01) 			//集合0：被地面控制
	{
		f_info.ctrl_mode=1;				//地面站控制

		if(*(data_buf+3)==0x01)			//起飞+定高
			f_info.Gnd_ctrl.act=up_kephig;
		else if(*(data_buf+3)==0x02)	//前进
			f_info.Gnd_ctrl.act=forward;
		else if(*(data_buf+3)==0x03)	//左转
			f_info.Gnd_ctrl.act=tn_lef;
		else if(*(data_buf+3)==0x04)	//右转
			f_info.Gnd_ctrl.act=tn_rig;
		else if(*(data_buf+3)==0x05)	//降高
			f_info.Gnd_ctrl.act=down;
		else if(*(data_buf+3)==0x06)	//升高
			f_info.Gnd_ctrl.act=up;
		else if(*(data_buf+3)==0x07)	//降落
			f_info.Gnd_ctrl.act=down_land;

		f_info.Gnd_ctrl.deg=*(data_buf+4);  //degree
	}
	else if(*(data_buf+2)==0x02)
	{
		f_info.ctrl_mode=2;				//通过队列控制
	}
	else
		f_info.ctrl_mode=0;
}
/***********串口1发送函数************************/
void Data_Transmit5_Gnd (uint8_t *data_buff)
{
	SCI5_Send(data_buff);
}
