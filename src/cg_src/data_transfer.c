/*
 * data_transfer.c
 *
 *  Created on: 2017年7月29日
 *      Author: LWD
 */
#include "data_transfer.h"




void Data_Receive_Anl (uint8_t *data_buf)
{
	unsigned char f_sta0,f_sta1,f_sta2;

	if( !((*data_buf==0xBB) && (*(data_buf+1)== 0xEE)) )
	{
		return;
	}
	if(*(data_buf+2)==0x00) 			//集合0：表示接收飞机目前飞行状态信息
	{
		f_sta2 = f_sta1;
		f_sta1 = f_sta0;
		switch(*(data_buf+3))
		{
				case 0x00:	f_sta0=0;  break;   //异常
				case 0x01:  f_sta0=1;  break;	//起飞中
				case 0x02:	f_sta0=2;  break;	//悬停中
				case 0x03:  f_sta0=3;  break;	//前进中
				case 0x04:	f_sta0=4;  break;	//降落中
				case 0x05:  f_sta0=5;  break;	//左转中
				case 0x06:	f_sta0=6; 	//右转中
		}
		if((f_sta0==f_sta1)&&(f_sta1==f_sta2))
				f_info.status = f_sta0;			//更新飞行状态
	}

}

