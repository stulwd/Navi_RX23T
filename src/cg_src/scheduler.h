/*
 * scheduler.h
 *
 *  Created on: 2017年8月3日
 *      Author: LWD
 */

#ifndef CG_SRC_SCHEDULER_H_
#define CG_SRC_SCHEDULER_H_



#endif /* CG_SRC_SCHEDULER_H_ */

#include "r_cg_macrodriver.h"

typedef struct
{
	uint8_t check_flag;
	uint8_t err_flag;
	uint8_t camera_data_ok;
	int16_t cnt_camera_data_ms;
	int16_t cnt_1ms;
	int16_t cnt_2ms;
	int16_t cnt_5ms;
	int16_t cnt_10ms;
	int16_t cnt_20ms;
	int16_t cnt_50ms;
	int16_t cnt_100ms;
	int16_t cnt_1s;
	int16_t cnt_2s;
	int16_t time;
}loop_t;
void Duty_Loop(void);
void Loop_check(void);
