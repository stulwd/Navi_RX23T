/*
 * duty.h
 *
 *  Created on: 2017年7月28日
 *      Author: LWD
 */

#ifndef CG_SRC_DUTY_H_
#define CG_SRC_DUTY_H_



#endif /* CG_SRC_DUTY_H_ */

#include "r_cg_macrodriver.h"

#define kp_posture		(uint8_t)0x00	//悬停
#define up_kephig		(uint8_t)0x01	//起飞+定高
#define forward			(uint8_t)0x02	//前进
#define tn_lef			(uint8_t)0x03	//左转
#define tn_rig			(uint8_t)0x04	//右转
#define down			(uint8_t)0x05	//降高
#define up				(uint8_t)0x06	//升高
#define down_land		(uint8_t)0x07	//降落
typedef struct{
	uint8_t act;						//action
	uint8_t deg;						//degree
}fly_Ctrl;

typedef struct{
	unsigned char status;				//目前飞行状态
	unsigned char ctrl_mode;			//控制模式
	fly_Ctrl Gnd_ctrl;					//地面手动控制模式
	fly_Ctrl Auto_ctrl;					//队列自动控制模式
}Fly_info;



extern Fly_info f_info;
extern uint8_t Mission_Buffer[20];			//任务队列
extern uint8_t Now_task;					//当前任务指针

void indicate_LED (void);
//void Navigate (void);
