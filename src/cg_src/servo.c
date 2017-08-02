/*
 * servo.c
 *
 *  Created on: 2017年8月2日
 *      Author: LWD
 */
#include "servo.h"
#include "r_cg_macrodriver.h"

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) );

void ctrl_servo(int angle)
{
	MTU0.TGRA=LIMIT( (2499.0/20.0)*(1.5+angle/360.0),0x04E1,0x09C2 );
}
