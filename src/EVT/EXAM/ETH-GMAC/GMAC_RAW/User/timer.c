/********************************** (C) COPYRIGHT *******************************
* File Name          : timer.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description        : ���ļ�����ʵ��CH569��ʱ����صĹ���
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "timer.h"
#include "core_riscv.h"
#include "string.h"
#include "stdio.h"

//extern UINT16 heatbeat;
//extern UINT8 systic;
//extern UINT8 globe_status;
//extern UINT8 Ping_Time;
//extern UINT8 net_mask_busy;
//extern UINT8 status;
volatile extern UINT8 enable_send;
volatile extern UINT8 rece_timeout_cnt;    /* ���ճ�ʱ��ʱ�� */
extern UINT32 TIMEOUT_CNT;                 /* ���ճ�ʱ�ƴ��� */

/* *****************************************************************************
 * @fn     TMR0_init
 *
 * @brief  ��ʱ����ʼ��
 *
 * @param ����ʱ����
 *
 * @return ����
 */
void TMR0_init(UINT32 n)
{
	R32_TMR0_CNT_END = n;                                     /* �����ն˼��� */
	R8_TMR0_CTRL_MOD = RB_TMR_MODE_IN;			              /* ʹ�ܼ���ģʽ */
	R8_TMR0_INTER_EN  |= RB_TMR_IE_CYC_END;		              /* ʹ�ܼ��������ж� */
	R8_TMR0_CTRL_MOD=RB_TMR_COUNT_EN;	                      /* ʹ�ܶ�ʱ�� */
	PFIC_EnableIRQ(TMR0_IRQn);                                /* ������ʱ���ж� */
}

/* *****************************************************************************
 * @fn    Net_TimerIsr
 *
 * @brief ��ÿ��һ���жϵĶ�ʱ���жϺ���
 *
 * @param ��ʱ����
 *
 * @return ����
 */
void Net_TimerIsr(void)
{
	UINT16 length;

	if(rece_timeout_cnt>=10)
	{
		enable_send=0x03;
		rece_timeout_cnt=0;
		TIMEOUT_CNT++;
		printf("T\n");
	}
	else
		rece_timeout_cnt++;

#if 0
	heatbeat++;
	if(heatbeat>=7200)
		heatbeat=0;
	if(Ping_Time)
	{
		systic++;
		if(systic>=10)
		{
			systic=0;
			printf("ping reply time out!\n");
			globe_status=1;
			status=1;
		}
	}
	if((heatbeat%120==0)&&(net_mask_busy==0))
	{
		request_local_ip(arp_buff,&length);
		memcpy(get_txbuff_addr(),arp_buff,length);
		mac_send(length);
	}
#endif
}

/* *****************************************************************************
 * @fn    TMR0_IRQHandler
 *
 * @brief ��ʱ���жϺ���
 *
 * @return ��
 */
void TMR0_IRQHandler(void)  __attribute__((interrupt("WCH-Interrupt-fast")));
void TMR0_IRQHandler(void)
{
	if(R8_TMR0_INT_FLAG & RB_TMR_IF_CYC_END)
	{
		R8_TMR0_INTER_EN |= RB_TMR_IF_CYC_END;
		R8_TMR0_INT_FLAG =  RB_TMR_IF_CYC_END;
		Net_TimerIsr();
	}
}



