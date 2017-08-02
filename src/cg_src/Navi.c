/*
 * duty.c
 *
 *  Created on: 2017年7月28日
 *      Author: LWD
 */


#include "Navi.h"
#include "r_cg_macrodriver.h"

unsigned char Tx_buffer[]="0123456789";
Fly_info f_info;



void Navigate ()
{




}












void Duty_1ms()
{
	;
}
void Duty_2ms()
{
	MTU0.TGRA++;
	if(MTU0.TGRA>=0x49C2)
	{
		MTU0.TGRA=0x04E1;
	}
}
void Duty_5ms()
{

}
void Duty_10ms()
{

}
void Duty_20ms()
{

}
void Duty_50ms()
{
	indicate_LED();     //飞机目前飞行状态指示
}
void Duty_1s()
{

}
void Duty_2s()
{


}




void indicate_LED()
{

	static unsigned char Tx_buffer[]="123456";
	static int cnt,led_f;

//	if(up_flag==1)
//	{led_f=4;up_flag=0;}
//	if(pass_A_flag==1)
//	{led_f=10;pass_A_flag=0;}
//	if(pass_B_flag==1)
//	{led_f=20;pass_B_flag=0;}
//	if(pass_C_flag==1)
//	{led_f=10;pass_C_flag=0;}
//	if(down_flag==1)
//	{led_f=4;down_flag=0;}

	R_SCI5_Serial_Send(Tx_buffer,6);
	if(cnt>=led_f)
	{
		PORT7.PODR.BIT.B6=~PORT7.PODR.BIT.B6;
		cnt=0;
	}
	cnt++;
}



