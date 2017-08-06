/*
 * oled.c
 *
 *  Created on: 2017年8月6日
 *      Author: LWD
 */


#include "oled.h"
#include "oledfont.h"


//更新显存到lcd


//oled的显存格式如下
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
uint8_t OLED_GRAM[128][8];

//更新显存到lcd
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置-列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置-列高地址
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}
}

//向SSD1306写入一个字节
//dat:要写入的数据/命令
//cmd：数据/命令-1/0
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
	uint8_t i;
	if(cmd)
	  OLED_RS_Set();
	else
	  OLED_RS_Clr();
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;
	}
	OLED_CS_Set();
	OLED_RS_Set();
}



//开启oled显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭oled显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//清屏函数，清完屏整个屏幕是黑色的
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
	OLED_Refresh_Gram();//¸üÐÂÏÔÊ¾
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
}



//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dat:0,清空;1,填充
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)
{
	uint8_t x,y;
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}
	OLED_Refresh_Gram();//¸üÐÂÏÔÊ¾
}
//在指定位置显示一个字符，包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint8_t y0=y;
	chr=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ
    for(t=0;t<size;t++)
    {
		if(size==12)temp=oled_asc2_1206[chr][t];  //µ÷ÓÃ1206×ÖÌå
		else temp=oled_asc2_1608[chr][t];		 //µ÷ÓÃ1608×ÖÌå
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
    }
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)result*=m;
	return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1);
	}
}
//显示字符串
//x,y:起点坐标
//*p:字符串起始地址
//用16字体
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58
    while(*p!='\0')
    {
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);
        x+=8;
        p++;
    }
}
void delay100ms(void)
{
unsigned char i,j,k;
for(i=15;i>0;i--)
for(j=202;j>0;j--)
for(k=81;k>0;k--);
}
//初始化SSD1306
void OLED_Init(void)
{
	OLED_RST_Clr();
	delay100ms();
	OLED_RST_Set();

	OLED_WR_Byte(0xAE,OLED_CMD); //¹Ø±ÕÏÔÊ¾
	OLED_WR_Byte(0xD5,OLED_CMD); //ÉèÖÃÊ±ÖÓ·ÖÆµÒò×Ó,Õðµ´ÆµÂÊ
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],·ÖÆµÒò×Ó;[7:4],Õðµ´ÆµÂÊ
	OLED_WR_Byte(0xA8,OLED_CMD); //ÉèÖÃÇý¶¯Â·Êý
	OLED_WR_Byte(0X3F,OLED_CMD); //Ä¬ÈÏ0X3F(1/64)
	OLED_WR_Byte(0xD3,OLED_CMD); //ÉèÖÃÏÔÊ¾Æ«ÒÆ
	OLED_WR_Byte(0X00,OLED_CMD); //Ä¬ÈÏÎª0

	OLED_WR_Byte(0x40,OLED_CMD); //ÉèÖÃÏÔÊ¾¿ªÊ¼ÐÐ [5:0],ÐÐÊý.

	OLED_WR_Byte(0x8D,OLED_CMD); //µçºÉ±ÃÉèÖÃ
	OLED_WR_Byte(0x14,OLED_CMD); //bit2£¬¿ªÆô/¹Ø±Õ
	OLED_WR_Byte(0x20,OLED_CMD); //ÉèÖÃÄÚ´æµØÖ·Ä£Ê½
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00£¬ÁÐµØÖ·Ä£Ê½;01£¬ÐÐµØÖ·Ä£Ê½;10,Ò³µØÖ·Ä£Ê½;Ä¬ÈÏ10;
	OLED_WR_Byte(0xA1,OLED_CMD); //¶ÎÖØ¶¨ÒåÉèÖÃ,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //ÉèÖÃCOMÉ¨Ãè·½Ïò;bit3:0,ÆÕÍ¨Ä£Ê½;1,ÖØ¶¨ÒåÄ£Ê½ COM[N-1]->COM0;N:Çý¶¯Â·Êý
	OLED_WR_Byte(0xDA,OLED_CMD); //ÉèÖÃCOMÓ²¼þÒý½ÅÅäÖÃ
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]ÅäÖÃ

	OLED_WR_Byte(0x81,OLED_CMD); //¶Ô±È¶ÈÉèÖÃ
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ä¬ÈÏ0X7F (ÁÁ¶ÈÉèÖÃ,Ô½´óÔ½ÁÁ)
	OLED_WR_Byte(0xD9,OLED_CMD); //ÉèÖÃÔ¤³äµçÖÜÆÚ
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //ÉèÖÃVCOMH µçÑ¹±¶ÂÊ
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //È«¾ÖÏÔÊ¾¿ªÆô;bit0:1,¿ªÆô;0,¹Ø±Õ;(°×ÆÁ/ºÚÆÁ)
	OLED_WR_Byte(0xA6,OLED_CMD); //ÉèÖÃÏÔÊ¾·½Ê½;bit0:1,·´ÏàÏÔÊ¾;0,Õý³£ÏÔÊ¾
	OLED_WR_Byte(0xAF,OLED_CMD); //¿ªÆôÏÔÊ¾
	OLED_Clear();
}
