/*
 * data_transfer.h
 *
 *  Created on: 2017年7月29日
 *      Author: LWD
 */

#ifndef CG_SRC_DATA_TRANSFER_H_
#define CG_SRC_DATA_TRANSFER_H_



#endif /* CG_SRC_DATA_TRANSFER_H_ */

#include "r_cg_macrodriver.h"



void Data_Receive1_Cop(uint8_t *data_buf);     //串口
void Data_Transmit1_Cop (uint8_t *data_buff);	//给Cop的发送
void Data_Receive5_Gnd(uint8_t *data_buf); 	    //数传
void Data_Transmit5_Gnd(uint8_t *data_buff);	//给地面端的发送
void data_trans(uint8_t act,uint8_t deg,uint8_t sci);
