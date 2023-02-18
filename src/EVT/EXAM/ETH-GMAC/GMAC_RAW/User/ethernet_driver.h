/********************************** (C) COPYRIGHT *******************************
* File Name          : ethernet_driver.h
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : CH565/569 ǧ����̫����·���շ��ӿڶ���
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#ifndef __ethernet_driver__h__
#define __ethernet_driver__h__

#include "CH56x_common.h"


#define  RX_Des_Num        8                                    /* �����������ͻ����������� */
#define  TX_Des_Num        2                                    /* �����������ͻ����������� */
#define  RX_Buf_Size       1520                                 /* ���ջ������Ĵ�С*/
#define  TX_Buf_Size       1520                                 /* ���ͻ������Ĵ�С*/

/* �Ƿ�ʹ��PHY�жϡ�����������PHY�ж���Ҫ����RST���ŵ�Ӳ����λ���ܲ�����GPIO�жϡ� */
#define USE_ETH_PHY_INTERRUPT    1                              /* Ĭ�ϲ�ʹ��PHY�ж� */

/* ���ù���1000BASE-Xģʽ����������ʹ�ù���ģʽʱ����Ҫǿ���ٶ�Ϊ1000M �����ע���¾䣬�������Ṥ����10BASE-T/100BASE-TX/1000BASE-T����Ӧ��ģʽ��*/
//#define FIBER_1000M

/* �����ڲ�125MHzʱ��Դ��ʹ���ڲ�ʱ��Դ�����USB���ֵĵ�Դ���ã���ע�⡣���ע���¾䣬������ʹ������ETCKI���ŵ�125MHz */
//#define USE_USB125M

/* �����ѡ������֧��RTL8211DN��FS�����ͺŵ���̫������㡣ÿ��ʹ��ֻ���ұ��붨������һ�� */
#define USE_RTL8211FS
//#define USE_RTL8211DN

#ifdef USE_RTL8211FS
	#define PHY_ADDRESS     0x01			/* RTL8211FS  PHYоƬ��ַ */
#endif
#ifdef USE_RTL8211DN
	#define PHY_ADDRESS     0x00			/* RTL821DN��PHYоƬ��ַ.�û���Ҫ����ʵ��ֵ�޸� */
#endif

#define Dscr_length  (16)                /* �������ĳ��ȣ���Ҫ������ */

/* CH565/569�ĸ���DMA��128λ��ģ���������Ҫ16�ֽڶ��롣�����޸Ĵ�ֵ */
#define Desc_buf_align   16              /* �������ͻ��������� */
//#define buff_base_addr   0x20020000

extern UINT8 MAC_QUEUE[];/* MACʹ�õĻ�����λ�ã���main.c������������룬��Ҫ16�ֽڶ��� */
//#define     ETH_queue_base           (volatile UINT8*)(buff_base_addr+20*1024)
//#define ETH_queue_base  (volatile UINT8*)(buff_base_addr)
#define ETH_queue_base  MAC_QUEUE

#if buff_base_addr%Desc_buf_align!=0
#error("Error��the base address of des and buff are not aligned by 16bytes!");
#endif

/* ����������ƫ�� */
#if Dscr_length%Desc_buf_align
	#define TxDscrOffset (Dscr_length/16+1)*16
#else
	#define TxDscrOffset Dscr_length
#endif

/* ����������ƫ�� */
#if Dscr_length%Desc_buf_align
	#define RxDscrOffset (Dscr_length/16+1)*16
#else
	#define RxDscrOffset Dscr_length
#endif

/* ���ͻ�����ƫ�� */
#define TxBufOffset TX_Des_Num*TX_Buf_Size

/* ����������ƫ�� */
#define RxBufOffset RX_Des_Num*RX_Buf_Size

/* ������������ַ */
#define pDMATxDscrTab (( ETH_DMADESCTypeDef*)ETH_queue_base)     /* ���������� */
/* ������������ַ */
#define pDMARxDscrTab (( ETH_DMADESCTypeDef*)(ETH_queue_base+TxDscrOffset*TX_Des_Num))     /* ���������� */
/* ���ͻ�������ַ */
#define	pTx_Buff      (( UINT8 *)(ETH_queue_base+TxDscrOffset*TX_Des_Num+RxDscrOffset*RX_Des_Num))     /* ���ջ����� */
/* ���ջ�������ַ */
#define	pRx_Buff      (( UINT8 *)(ETH_queue_base+TxDscrOffset*TX_Des_Num+RxDscrOffset*RX_Des_Num+TX_Des_Num*TX_Buf_Size))     /* ���ͻ����� */


extern ETH_DMADESCTypeDef *DMARxDscrTab;			//��̫��DMA�������������ݽṹ��ָ��
extern ETH_DMADESCTypeDef *DMATxDscrTab;			//��̫��DMA�������������ݽṹ��ָ�� 
extern UINT8 *Rx_Buff; 							//��̫���ײ���������buffersָ��
extern UINT8 *Tx_Buff; 							//��̫���ײ���������buffersָ��
extern ETH_DMADESCTypeDef  *DMATxDescToSet;			//DMA����������׷��ָ��
extern ETH_DMADESCTypeDef  *DMARxDescToGet; 		//DMA����������׷��ָ�� 

typedef enum
{
	ETH_disconnect=0,
	ETH_10M       =1,
	ETH_100M      =2,
	ETH_Gigabit   =3,
} Link_Speed_t;
//void	mDelayuS( unsigned short n );
//void	mDelaymS(  unsigned short n );

void ETH_GPIO_Init(void);
void RGMII_TXC_Delay(UINT8 clock_polarity,UINT8 delay_time);
UINT8 ETH_Mem_Malloc(void);
Link_Speed_t rtl8211dn_Get_Speed(void);
UINT8 ETH_buf_init(void);
void ENABLE_ETH_WAKEUP(void);
UINT8 ETH_MACDMA_Config(void);
UINT8 ETH_MACDMA_reConfig(void);
UINT8 Mac_init(void);
void ETH_init(void);
void ENABLE_PMT_INT(void);
void Check_TxDes(void);
UINT8 ETH_Tx_Packet(unsigned short FrameLength);
UINT32 ETH_GetCurrentTxBuffer(void);

void ETH_StructInit(ETH_InitTypeDef* ETH_InitStruct);

UINT8 mac_send(UINT32 length);
void MAC_ReInit(ETH_InitTypeDef* ETH_InitStruct);
UINT16 mac_rece(UINT8 ** ptr/*,UINT16 *rece_lengeh*/);
void ETH_DMAClearITPendingBit(UINT32 ETH_DMA_IT);
void ETH_DMAITConfig(UINT32 ETH_DMA_IT, FunctionalState NewState);
void ETH_MACAddressConfig(UINT32 MacAddr, UINT8 *Addr);
void ETH_DMARxDescChainInit(ETH_DMADESCTypeDef *DMARxDescTab, UINT8 *RxBuff, UINT32 RxBuffCount);
void ETH_DMATxDescChainInit(ETH_DMADESCTypeDef *DMATxDescTab, UINT8* TxBuff, UINT32 TxBuffCount);
UINT32 ETH_Init(ETH_InitTypeDef* ETH_InitStruct, UINT16 PHYAddress);
void ETH_StructInit(ETH_InitTypeDef* ETH_InitStruct);
void ETH_SoftwareReset(void);
FlagStatus ETH_GetSoftwareResetStatus(void);
UINT16 ETH_ReadPHYRegister(UINT16 PHYAddress, UINT16 PHYReg);
UINT32 ETH_WritePHYRegister(UINT16 PHYAddress, UINT16 PHYReg, UINT16 PHYValue);

/* ȫ�ֽ���״̬������������̫����������״̬  */
typedef struct
{
	ETH_DMADESCTypeDef *LastRxDes; /* ָ�����һ�δ�����Ľ�������������һ���������ĵ�ַ�����Ǵ�����Ľ����������ĵ�ַ */
	UINT16 pengding_RxDes_cnt;     /* ������Ľ����������ĸ������ڽ����ж����������ڴ����������Լ� ��������ֵ���ڽ��ն�����ȣ����Ѿ��������� */
#if 1
	UINT16 package_loss_cnt;       /* ��ʾ��������Ż�����ʹ������ٶ����⣬���µĶ����������ⲻ�Ƕ����İ��� */
#endif
}Globe_RxDes_status_t;


//#define if_any_frames_not_read()  (Globe_RxDes_status.pengding_RxDes_cnt)
#define get_txbuff_addr()         ((UINT8*)(DMATxDescToSet->Buffer1Addr))

#endif 

