/*
 * oled.h
 *
 *  Created on: 2017年8月6日
 *      Author: LWD
 */

#ifndef CG_SRC_OLED_H_
#define CG_SRC_OLED_H_

#endif /* CG_SRC_OLED_H_ */
#include "r_cg_macrodriver.h"


#define OLED_MODE 0	//模式0 串行工作方式




//-----------------OLED¶Ë¿Ú¶¨Òå----------------

#define OLED_CS_Clr()  PORT4.PODR.BIT.B5 = 0U
#define OLED_CS_Set()  PORT4.PODR.BIT.B5 = 1U

#define OLED_RST_Clr() PORT4.PODR.BIT.B2 = 0U
#define OLED_RST_Set() PORT4.PODR.BIT.B2 = 1U

#define OLED_RS_Clr()  PORT4.PODR.BIT.B7 = 0U
#define OLED_RS_Set()  PORT4.PODR.BIT.B7 = 1U

//#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
//#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)
//
//#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
//#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)



////PC0~7,×÷ÎªÊý¾ÝÏß
//#define DATAOUT(x) GPIO_Write(GPIOC,x);//Êä³ö
////Ê¹ÓÃ4Ïß´®ÐÐ½Ó¿ÚÊ±Ê¹ÓÃ

#define OLED_SCLK_Clr() PORT4.PODR.BIT.B6 = 0U
#define OLED_SCLK_Set() PORT4.PODR.BIT.B6 = 1U

#define OLED_SDIN_Clr() PORT4.PODR.BIT.B4 = 0U
#define OLED_SDIN_Set() PORT4.PODR.BIT.B4 = 1U

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据




void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);

void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p);
