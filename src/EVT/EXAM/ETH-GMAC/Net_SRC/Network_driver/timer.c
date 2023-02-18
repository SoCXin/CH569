/********************************** (C) COPYRIGHT *******************************
* File Name          : timer.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include <CH569Net_lib.h>                         /* ��ͷ�ļ� */
#include "timer.h"
#include "core_riscv.h"
#include "string.h"
#include "stdio.h"


UINT16 globe_send_flag_cnt;

/* *****************************************************************************
 * @fn     TMR0_init
 *
 * @brief ��ʱ����ʼ��
 *
 * @param  ��ʱ����
 *
 * @return  ��
 */
void TMR0_init(UINT32 n)
{
	R32_TMR0_CNT_END = n;                                     /* �����ն˼��� */
	R8_TMR0_CTRL_MOD = RB_TMR_MODE_IN;			              /* ʹ�ܼ���ģʽ */
	R8_TMR0_INTER_EN  |= RB_TMR_IE_CYC_END;		              /* ʹ�ܼ��������ж� */
	R8_TMR0_CTRL_MOD=RB_TMR_COUNT_EN;	                      /* ʹ�ܶ�ʱ�� */
	PFIC_SetPriority(TMR0_IRQn,0xa0);                         /* �����ȼ� */
	PFIC_EnableIRQ(TMR0_IRQn);                                /* ������ʱ���ж� */
}

/* *****************************************************************************
 * @fn    Net_TimerIsr
 *
 * @briefÿ��һ���жϵĶ�ʱ���жϺ���
 *
 * @param ��ʱ����
 *
 * @return  ��
 **/
/* ������ֵΪϵͳ��Ƶ����100������£�ÿ10�����һ���ж� */
void Net_TimerIsr(void)
{
    CH569NET_TimeIsr(CH569NETTIMEPERIOD);                                  /* ��ʱ���жϷ����� */

}
//static UINT32 LINSHI;
/* *****************************************************************************
 * @fn    TMR0_IRQHandler
 *
 * @brief ��ʱ���жϺ���
 *
 * @return  ��
 */
void TMR0_IRQHandler(void)  __attribute__((interrupt("WCH-Interrupt-fast")));
void TMR0_IRQHandler(void)
{
	if(R8_TMR0_INT_FLAG & RB_TMR_IF_CYC_END)
	{
		R8_TMR0_INTER_EN |= RB_TMR_IF_CYC_END;
		R8_TMR0_INT_FLAG =  RB_TMR_IF_CYC_END;
		Net_TimerIsr();
		globe_send_flag_cnt++;
	}
}


//Ϊ�˲���ʱ�俪��ʱ��
void tmr1_init(void)
{
	R32_TMR1_CNT_END=0x3FFFFFF;
	R8_TMR1_CTRL_MOD = RB_TMR_MODE_IN;
	R8_TMR1_CTRL_MOD = RB_TMR_COUNT_EN;
}

unsigned int gettime(void)
{
	unsigned int a=0;
	a=R32_TMR1_COUNT;
	return a;
}





