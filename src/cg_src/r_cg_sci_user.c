/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sci_user.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2017/8/4
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
//,RxBuffer_1[10],RxBuffer_2[10],RxBuffer_3[10];
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
#include "data_transfer.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t * gp_sci1_tx_address;                /* SCI1 send buffer address */
extern uint16_t  g_sci1_tx_count;                   /* SCI1 send data number */
extern uint8_t * gp_sci1_rx_address;                /* SCI1 receive buffer address */
extern uint16_t  g_sci1_rx_count;                   /* SCI1 receive data number */
extern uint16_t  g_sci1_rx_length;                  /* SCI1 receive data length */
extern uint8_t * gp_sci5_tx_address;                /* SCI5 send buffer address */
extern uint16_t  g_sci5_tx_count;                   /* SCI5 send data number */
extern uint8_t * gp_sci5_rx_address;                /* SCI5 receive buffer address */
extern uint16_t  g_sci5_rx_count;                   /* SCI5 receive data number */
extern uint16_t  g_sci5_rx_length;                  /* SCI5 receive data length */
/* Start user code for global. Do not edit comment generated here */
unsigned char Com_Data1;		//串口接收数据
uint8_t TXcount1=0;				//发送队列的发送指针
uint8_t count1=0;				//发送队列的入指针
uint8_t TxBuffer1[100][10];		//发送buff
uint8_t send_stop_flag1 = 1;	//队列发空后置1
unsigned char Com_Data5;		//数传接收数据
uint8_t TXcount5=0;				//发送队列的发送指针
uint8_t count5=0;				//发送队列的入指针
uint8_t TxBuffer5[100][10];		//发送buff
uint8_t send_stop_flag5 = 1;	//队列发空后置1
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_sci1_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TXI1
#pragma interrupt r_sci1_transmit_interrupt(vect=VECT(SCI1,TXI1),fint)
#else
#pragma interrupt r_sci1_transmit_interrupt(vect=VECT(SCI1,TXI1))
#endif
static void r_sci1_transmit_interrupt(void)
{
    if (0U < g_sci1_tx_count)
    {
        SCI1.TDR = *gp_sci1_tx_address;
        gp_sci1_tx_address++;
        g_sci1_tx_count--;
    }
    else
    {
        SCI1.SCR.BIT.TIE = 0U;
        SCI1.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_sci1_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TEI1
#pragma interrupt r_sci1_transmitend_interrupt(vect=VECT(SCI1,TEI1),fint)
#else
#pragma interrupt r_sci1_transmitend_interrupt(vect=VECT(SCI1,TEI1))
#endif
static void r_sci1_transmitend_interrupt(void)
{
    /* Set TXD1 pin */
    PORTD.PMR.BYTE &= 0xF7U;
    SCI1.SCR.BIT.TIE = 0U;
    SCI1.SCR.BIT.TE = 0U;
    SCI1.SCR.BIT.TEIE = 0U;

    r_sci1_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci1_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_RXI1
#pragma interrupt r_sci1_receive_interrupt(vect=VECT(SCI1,RXI1),fint)
#else
#pragma interrupt r_sci1_receive_interrupt(vect=VECT(SCI1,RXI1))
#endif
static void r_sci1_receive_interrupt(void)
{
    if (g_sci1_rx_length > g_sci1_rx_count)
    {
        *gp_sci1_rx_address = SCI1.RDR;
        gp_sci1_rx_address++;
        g_sci1_rx_count++;

        if (g_sci1_rx_length <= g_sci1_rx_count)
        {
            r_sci1_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_sci1_receiveerror_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_ERI1
#pragma interrupt r_sci1_receiveerror_interrupt(vect=VECT(SCI1,ERI1),fint)
#else
#pragma interrupt r_sci1_receiveerror_interrupt(vect=VECT(SCI1,ERI1))
#endif
static void r_sci1_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_sci1_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI1.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI1.SSR.BYTE = err_type;
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_transmitend
* Description  : This function is a callback function when SCI1 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
	TXcount1++;
	if(TXcount1==99) TXcount1 = 0;

	if(TXcount1==count1)
	{
		send_stop_flag1 = 1;
	}
	else
		R_SCI1_Serial_Send(&TxBuffer1[TXcount1][0],10);
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_receiveend
* Description  : This function is a callback function when SCI1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
	R_SCI1_Serial_Receive(&Com_Data1 , 1);
	Receive1_joint(Com_Data1);
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_receiveerror
* Description  : This function is a callback function when SCI1 reception encounters error.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_receiveerror(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci5_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TXI5
#pragma interrupt r_sci5_transmit_interrupt(vect=VECT(SCI5,TXI5),fint)
#else
#pragma interrupt r_sci5_transmit_interrupt(vect=VECT(SCI5,TXI5))
#endif
static void r_sci5_transmit_interrupt(void)
{
    if (0U < g_sci5_tx_count)
    {
        SCI5.TDR = *gp_sci5_tx_address;
        gp_sci5_tx_address++;
        g_sci5_tx_count--;
    }
    else
    {
        SCI5.SCR.BIT.TIE = 0U;
        SCI5.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_sci5_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TEI5
#pragma interrupt r_sci5_transmitend_interrupt(vect=VECT(SCI5,TEI5),fint)
#else
#pragma interrupt r_sci5_transmitend_interrupt(vect=VECT(SCI5,TEI5))
#endif
static void r_sci5_transmitend_interrupt(void)
{
    /* Set TXD5 pin */
    PORTB.PMR.BYTE &= 0xDFU;
    SCI5.SCR.BIT.TIE = 0U;
    SCI5.SCR.BIT.TE = 0U;
    SCI5.SCR.BIT.TEIE = 0U;

    r_sci5_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci5_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_RXI5
#pragma interrupt r_sci5_receive_interrupt(vect=VECT(SCI5,RXI5),fint)
#else
#pragma interrupt r_sci5_receive_interrupt(vect=VECT(SCI5,RXI5))
#endif
static void r_sci5_receive_interrupt(void)
{
    if (g_sci5_rx_length > g_sci5_rx_count)
    {
        *gp_sci5_rx_address = SCI5.RDR;
        gp_sci5_rx_address++;
        g_sci5_rx_count++;

        if (g_sci5_rx_length <= g_sci5_rx_count)
        {
            r_sci5_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_sci5_receiveerror_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_ERI5
#pragma interrupt r_sci5_receiveerror_interrupt(vect=VECT(SCI5,ERI5),fint)
#else
#pragma interrupt r_sci5_receiveerror_interrupt(vect=VECT(SCI5,ERI5))
#endif
static void r_sci5_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_sci5_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI5.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI5.SSR.BYTE = err_type;
}
/***********************************************************************************************************************
* Function Name: r_sci5_callback_transmitend
* Description  : This function is a callback function when SCI5 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci5_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
	TXcount5++;
	if(TXcount5==99) TXcount5 = 0;

	if(TXcount5==count5)
	{
		send_stop_flag5 = 1;
	}
	else
		R_SCI5_Serial_Send(&TxBuffer5[TXcount5][0],10);
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci5_callback_receiveend
* Description  : This function is a callback function when SCI5 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci5_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
	R_SCI5_Serial_Receive(&Com_Data5 , 1);
	Receive5_joint(Com_Data5);
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci5_callback_receiveerror
* Description  : This function is a callback function when SCI5 reception encounters error.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci5_callback_receiveerror(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/*******************串口1接收数据拼接函数*****************************************************************/
void Receive1_joint(uint8_t data)
{
	static uint8_t _data_len = 0,_data_cnt = 0;
	static uint8_t state = 0;
	static unsigned char RxBuffer[10];		//接收buff

	if(state==0&&data==0xBB)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xEE)
	{
		state=2;
		RxBuffer[1]=data;
		_data_len=8;
	}
	else if(state==2&&_data_len>0)
	{
		_data_len--;
		RxBuffer[2+_data_cnt++]=data;
		if(_data_len==0)
		{
			state = 0;
			_data_cnt=0;
			Data_Receive1_Cop(RxBuffer);
		}
	}
	else
		state = 0;
}


/***串口1发送函数***/
SCI1_Send(unsigned char *DataToSend)
{
	uint8_t i;
	for(i=0;i<10;i++)
	{
		TxBuffer1[count1][i] = *(DataToSend+i);			//入队列
	}
	count1++;
	if(count1==100) count1=0;        //队列循环


	if(send_stop_flag1 == 1)								//重新开启发送
	{
		R_SCI1_Serial_Send(&TxBuffer1[TXcount1][0],10);    //发送
		send_stop_flag1 = 0;
	}

}





/*******************数传5接收数据拼接函数****************************************************************/

void Receive5_joint(uint8_t data)
{
	static uint8_t _data_len = 0,_data_cnt = 0;
	static uint8_t state = 0;
	static unsigned char RxBuffer[10];

	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xEE)
	{
		state=2;
		RxBuffer[1]=data;
		_data_len=8;
	}
	else if(state==2&&_data_len>0)
	{
		_data_len--;
		RxBuffer[2+_data_cnt++]=data;
		if(_data_len==0)
		{
			state = 0;
			_data_cnt=0;
			Data_Receive5_Gnd(RxBuffer);
		}
	}
	else
		state = 0;
}

/**********数传5发送******************/
SCI5_Send(unsigned char *DataToSend)
{
	uint8_t i;
	for(i=0;i<10;i++)
	{
		TxBuffer5[count5][i] = *(DataToSend+i);			//入队列
	}
	count5++;
	if(count5==100) count5=0;        //队列循环


	if(send_stop_flag5 == 1)								//重新开启发送
	{
		R_SCI5_Serial_Send(&TxBuffer5[TXcount5][0],10);    //发送
		send_stop_flag5 = 0;
	}

}

/* End user code. Do not edit comment generated here */
