/*
 * duty.h
 *
 *  Created on: 2017年7月28日
 *      Author: LWD
 */

#ifndef CG_SRC_DUTY_H_
#define CG_SRC_DUTY_H_



#endif /* CG_SRC_DUTY_H_ */

typedef struct{
	unsigned char status;
}Fly_info
;
extern Fly_info f_info;

void Duty_1ms(void);
void Duty_2ms(void);
void Duty_5ms(void);
void Duty_10ms(void);
void Duty_20ms(void);
void Duty_50ms(void);
void Duty_1s(void);
void Duty_2s(void);

void indicate_LED (void);
void Navigate (void);
