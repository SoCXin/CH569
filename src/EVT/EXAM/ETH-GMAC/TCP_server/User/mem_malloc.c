/********************************** (C) COPYRIGHT *******************************
* File Name          : mem_malloc.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : CH565/569 ǧ����̫���շ����շ����м� TCP/IPЭ��ջ�ڴ����뺯��
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include <CH569Net_lib.h>                         /* ��ͷ�ļ� */
#include "ethernet_config.h"
#include "ethernet_driver.h"


/*-------------------------------MAC%DMA�ռ�-----------------------------------------------------*/

/* MAC�շ������ڴ����룬����ռ���������շ����к������� */
__attribute__ ((aligned(16)))
UINT8 MAC_QUEUE[TxDscrOffset*TX_Des_Num+                     /* ������������Ҫ�Ŀռ� */
				RxDscrOffset*RX_Des_Num+                     /* ������������Ҫ�Ŀռ� */
				TX_Des_Num/*1*/*TX_Buf_Size+                 /* ���ͻ�������Ҫ�Ŀռ� */   //���ͻ���������1  by ltp
				RX_Des_Num*RX_Buf_Size]={0};                 /* ���ջ�������Ҫ�Ŀռ� */

/* ������ȫ��ָ������ָʾ�����ǰʹ�õ���������ע���CHRDR/CHTDR���� */
 ETH_DMADESCTypeDef  *DMATxDescToSet;                         /* ��ǰ����������ָ�� */
 ETH_DMADESCTypeDef  *DMARxDescToGet;                         /* ��ǰ����������ָ�� */


/*-----------------------------Э��ջ����ռ�--------------------------*/
__attribute__ ((aligned(4)))
SOCK_INF SocketInf[CH569NET_MAX_SOCKET_NUM];                  /* Socket��Ϣ��4�ֽڶ��룬���Ը���ʵ��ʹ�õ�socket���� */

/*ԭ������Ϊ8,������4�����ڴ�ع����ڴ�ѵ�ÿ�������MemNum�ж��� */
const UINT16 MemNum[12]  = {MemNum_content};                   /* һЩ�̶��Ĳ��� */
const UINT16 MemSize[12] = {MemSize_content};                  /* һЩ�̶��Ĳ��� */

/* �ڴ�ؿռ� *//* Э��ջ��Ҫ�ĳؿռ� */
__attribute__ ((aligned(4)))
UINT8 Memp_Memory[CH569NET_MEMP_SIZE]={0};


#if 0
/* Э��ջ��Ҫ�Ķѿռ� */
__attribute__ ((aligned(4)))//__attribute__((section(".dmadata")))
UINT8 Mem_Heap_Memory[CH569NET_RAM_HEAP_SIZE]={0};
#endif

/* Э��ջ��Ҫ��ARP��ռ� */
__attribute__ ((aligned(4)))
UINT8 Mem_ArpTable[CH569NET_RAM_ARP_TABLE_SIZE]={0};








