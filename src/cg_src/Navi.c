/*
 * duty.c
 *
 *  Created on: 2017年7月28日
 *      Author: LWD
 */

#include "Navi.h"
#include "data_transfer.h"
#include "servo.h"


Fly_info f_info;
uint8_t  Mission_Buffer[20];
uint8_t  Now_task=0;

/************************************************************************************************************************
*
*		#define kp_posture		(uint8_t)0x00	//悬停
*		#define up_kephig		(uint8_t)0x01	//起飞+定高
*		#define forward			(uint8_t)0x02	//前进
*		#define tn_lef			(uint8_t)0x03	//左转
*		#define tn_rig			(uint8_t)0x04	//右转
*		#define down			(uint8_t)0x05	//降高
*		#define up				(uint8_t)0x06	//升高
*		#define down_land		(uint8_t)0x07	//降落
*
*		#define unlock			(uint8_t)0x10	//解锁
*		#define unlock			(uint8_t)0x11	//锁定
*
*		case 0x00:	f_info.status=0;  break;   	//异常
*		case 0x01:  f_info.status=1;  break;	//起飞+定高完成
*		case 0x02:	f_info.status=2;  break;	//前进完成
*		case 0x03:  f_info.status=3;  break;	//左转完成
*		case 0x04:	f_info.status=4;  break;	//右转完成
*		case 0x05:  f_info.status=5;  break;	//降高完成
*		case 0x06:	f_info.status=6;  break; 	//升高完成
*		case 0x07:  f_info.status=7;			//降落
***************************************************************************************************************************/
void Navigate ()
{
	if(f_info.status == Mission_Buffer[Now_task])		//判断若当前任务完成，进入下一个任务
	{
		Now_task++;
	}
	f_info.Auto_ctrl.act= Mission_Buffer[Now_task];
}

