/*
 * key.c
 *
 *  Created on: 2017年8月3日
 *      Author: LWD
 */


#include "key.h"
#include "r_cg_icu.h"
void mode_setting()
{
	while (1U)
	{
		if((set_finishfuck==1U))
		{
			break;
		}
	}	//设置完成后关闭外部中断
	R_ICU_IRQ0_Stop();
	R_ICU_IRQ1_Stop();
	R_ICU_IRQ2_Stop();
	R_ICU_IRQ4_Stop();
	R_ICU_IRQ5_Stop();
}
