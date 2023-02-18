/********************************** (C) COPYRIGHT *******************************
* File Name          : PHY_interrupt.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : CH565/569 ǧ����̫��������ж���ʾ����
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "CH56x_common.h"
#include "ethernet_driver.h"
#include "ethernet_config.h"
#if USE_ETH_PHY_INTERRUPT /* ���������ʹ��������ж� */
#include "PHY_interrupt.h"

globe_eth_status_t globe_eth_status; /* ȫ����̫��״̬ */

/*******************************************************************************
 * @fn       phy_int_cheak_init
 *
 * @brief    Initializes the GPIO interrupt(PB15).
 *
 * @return   None
 **/
void phy_int_cheak_init(void)
{
	UINT16 RegValue;

	/* ʹ��PB15�͵�ƽ�ж� */
	GPIOB_ITModeCfg( 15, GPIO_ITMode_LowLevel );          /* PB15��ΪGPIO�жϴ������ţ��͵�ƽ�����ж�  */
	R32_PB_PU = (1<<15);                                  /* PB15���� */
	R8_GPIO_INT_FLAG = 0xff;                              /* ���жϱ�־λ */
	PFIC_EnableIRQ(GPIO_IRQn);                            /* ʹ��GPIO�ж� */

	/* ʹ��PHY���ж� */
#ifdef USE_RTL8211FS
    ETH_WritePHYRegister(PHY_ADDRESS, 31,0x0a42 );        /* �л�ҳ */
    RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, 0x12);    /* ��ȡ�����INER�Ĵ��� */
    ETH_WritePHYRegister(PHY_ADDRESS, 0x12,0xffff );      /* дINER�Ĵ��� */
    /* ��ȡ��ʼ�ж�״̬������ */
    ETH_WritePHYRegister(PHY_ADDRESS, 31,0x0a43 );        /* �л�ҳ */
    ETH_ReadPHYRegister(PHY_ADDRESS, 0x1d);
    /* ���������INSR�Ĵ��� */
    RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, 0x1a);
    /* д��ʼ������״̬ */
	if(RegValue & 0x0008)
	{
		globe_eth_status.phy_link_duples=full_duples;
		printf("phy ini init:full_duples.\n");
	}
	else
	{
		globe_eth_status.phy_link_duples=half_duples;
		printf("phy ini init:half_duples.\n");
	}
	if(RegValue & 0x0004)
	{
		globe_eth_status.phy_link_status=link_ok;
		printf("phy ini init:link_ok.\n");
	}
	else
	{
		globe_eth_status.phy_link_status=link_not_ok;
		printf("phy ini init:link_not_ok.\n");
	}
	if( RegValue & 0x0030 == 0x0000 )
	{
		globe_eth_status.phy_link_speed=linked_10M;
		printf("phy ini init:initial link speed:10M.\n");
	}
	else if(RegValue & 0x0030 == 0x0010)
	{
		globe_eth_status.phy_link_speed=linked_100M;
		printf("phy ini init:initial link speed:100M.\n");
	}
	else if(RegValue & 0x0030 == 0x0020)
	{
		globe_eth_status.phy_link_speed=linked_1000M;
		printf("phy ini init:initial link speed:1000M.\n");
	}
#endif
}

/*******************************************************************************
 * @fn       cheak_phy_int
 *
 * @brief    cheak phy interrupt.
 *
 * @return   None
 */
void cheak_phy_int(void)
{
	UINT16 RegValue;

#ifdef USE_RTL8211FS
    ETH_WritePHYRegister(PHY_ADDRESS, 31,0x0a43 );    /* �л�ҳ */
    RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, 0x1d);/* ��ȡ�����INSR�Ĵ��� */
    if(RegValue & 0x0001)                               /* �Զ�Э�̳��� */
    {
    	printf("phy int:auto-negotiation error!\n");
    }
    if(RegValue & 0x0008)                               /* �Զ�Э����� */
    {
//    	printf("phy int:auto-negotiation completed!\n");
    }
    if(RegValue & 0x0080)                               /* ������⵽PME/WOL�¼� */
    {
    	printf("phy int:PME/WOL event.\n");
    }
    if(RegValue & 0x0010)                               /* ���������״̬�ı� */
    {
    	globe_eth_status_t tem_eth_status;              /* ʵʱ����״̬ */

        RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, 0x1a);
    	printf("phy int:link status changed!physr:0x%04x.\n",RegValue);
        /* ��ȡ���µ�����״̬ */
		if(RegValue & 0x0004)
		{
			tem_eth_status.phy_link_status=link_ok;
			globe_eth_status.phy_link_status=link_ok;
			printf("phy int:eth linked!\n");
		}
		else
		{
			tem_eth_status.phy_link_status=link_not_ok;
			globe_eth_status.phy_link_status=link_not_ok;
			printf("phy int:eth disconnect!\n");
		}
		if(tem_eth_status.phy_link_status == link_ok)
		{
			if(RegValue & 0x0008)
			{
				tem_eth_status.phy_link_duples=full_duples;
				if( tem_eth_status.phy_link_duples != globe_eth_status.phy_link_duples )
				{
					printf("link duples changed!.full_duples");
					globe_eth_status.phy_link_duples = full_duples;
				}
			}
			else
			{
				tem_eth_status.phy_link_duples=half_duples;
				if( tem_eth_status.phy_link_duples != globe_eth_status.phy_link_duples )
				{
					printf("phy int:link status has changed to half_duples!\n   Error��CH565/569 does not suppose half_duples!\n");
					globe_eth_status.phy_link_duples = half_duples;
				}
			}
			if( (RegValue & 0x0030) == 0x0000 )
			{
				tem_eth_status.phy_link_speed=linked_10M;
			}
			if( (RegValue & 0x0030) == 0x0010)
			{
				tem_eth_status.phy_link_speed=linked_100M;
			}
			if( (RegValue & 0x0030) == 0x0020)
			{
				tem_eth_status.phy_link_speed=linked_1000M;
			}
			/* ��ȫ��״̬���Ƚ� */
			if(globe_eth_status.phy_link_speed !=  tem_eth_status.phy_link_speed)
			{
				globe_eth_status.phy_link_speed = tem_eth_status.phy_link_speed;
				printf("phy int:link speed changed!\nrealtime link speed: %d Mbps.\n",tem_eth_status.phy_link_speed);
//				globe_eth_status.ETH_reinit_flag|=8;/* ��Ҫ���³�ʼ�� */
				while(ETH_MACDMA_reConfig()!=1)
				{
					printf("Ethernet restart failed!retrying..\n");
				}
				printf("ETH_MACDMA_reConfig finished!\n");
			}
		}
    	printf("\n");
    }
#endif
}

/*******************************************************************************
 * @fn      GPIO_IRQHandler
 *
 * @brief   This function handles GPIO exception.
 *
 * @return  None
 */
void GPIO_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void GPIO_IRQHandler(void)
{
//	printf("GPIO_IRQHandler.\n");
	if(GPIOB_15_ReadITFlagBit())
	{
		cheak_phy_int();
	}
	GPIOB_15_ClearITFlagBit();
}

#endif
