/*
 * data_transfer.c
 *
 *  Created on: 2017年7月29日
 *      Author: LWD
 */
#include "data_transfer.h"




void Data_Receive_Anl (uint8_t *data_buf)
{


	if( !((*data_buf==0xBB) && (*(data_buf+1)== 0xEE)) )
	{
		return;
	}


	if(*(data_buf+2)==0x01)		//命令集合1（飞机位置状态信息）
	{
		if(*(data_buf+3)==0x01)				//0x01：飞机正在起飞     O---|------|---*
		{
			up_flag=1;
		}
		else if(*(data_buf+3)==0x02)		//0x02：经过路段1	 O---|------|--*-O
		{
			pass_A_flag=1;
		}
		else if(*(data_buf+3)==0x03)		//0x03：经过路段2	O---|---*---|---O
		{
			pass_B_flag=1;

		}
		else if(*(data_buf+3)==0x04)		//0x04: 经过路段3    O-*--|-----|---O
		{	pass_C_flag=1;

		}
		else if(*(data_buf+3)==0x05)		//0x04: 飞机正在降落    *---|-----|---O
		{
			down_flag=1;
		}
	}
	/*
	if(*(data_buf+2)==0x02)		//命令集合1（功能字为01）
	{
		if(*(data_buf+4)==0x01)				//0x01：ACC校准
		{
			mpu6050.Acc_CALIBRATE = 1;
		}
		else if(*(data_buf+4)==0x02)		//0x02：GYRO校准
		{
			mpu6050.Gyro_CALIBRATE = 1;
		}
	}

	if(*(data_buf+2)==0x03)		//命令集合1（功能字为01）
	{
		if(*(data_buf+4)==0x01)				//0x01：ACC校准
		{
			mpu6050.Acc_CALIBRATE = 1;
		}
		else if(*(data_buf+4)==0x02)		//0x02：GYRO校准
		{
			mpu6050.Gyro_CALIBRATE = 1;
		}
	}
*/
}

