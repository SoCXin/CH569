/********************************** (C) COPYRIGHT *******************************
* File Name          : ethernet_driver.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : CH565/569 ǧ����̫����·���շ��ӿں���ʵ��
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

/*-----------------------------------------------ͷ�ļ�����----------------------------- -------------*/
#include "ethernet_driver.h"
#include "CH56x_common.h"
#include "ethernet_config.h"

#ifdef USE_USB125M/* ʹ��ʹ���ڲ�125MHzʱ�� */
#include "usbss.h"
#endif

#ifdef USE_ETH_PHY_INTERRUPT
#include "PHY_interrupt.h"
#endif
/*-----------------------------------------------�궨��-----------------------------------------------*/


/*----------------------------------------------ȫ�ֱ���-----------------------------------------------*/
volatile  Globe_RxDes_status_t Globe_RxDes_status;/* ȫ�ֽ���״̬ */
extern UINT8  local_mac[];/* ����MAC��ַ��������main.c�� */
volatile extern UINT8 enable_send;
volatile extern UINT8 rece_timeout_cnt;    /* ���ճ�ʱ��ʱ�� */

#ifdef USE_ETH_PHY_INTERRUPT
extern globe_eth_status_t globe_eth_status; /* ȫ����̫��״̬ */
#endif

/*******************************************************************************
 * @fn      ETH_GPIO_Init
 *
 * @brief   ETH GPIO initialization
 *
 * @return  None
 */
void ETH_GPIO_Init(void)
{
#ifdef USE_RMII
//	(*((PUINT8V)0x40001000+0x12))&=~0x01;/* ʹ��RMII�ӿ� */
	R8_PIN_ALTERNATE&=~RB_PIN_MII;/* ʹ��RMII�ӿ� */
#endif
	/* ��Ҫע��CH565/569��MII�ӿڡ�SMI�ӿڡ�125Mʱ�ӵ������EMCO������ܽŵķ���  */
	/* PB13 RXDV / PB12 RXD0 / PB11 RXD1 / PB10 RXD2 / PB9 RXD3 / PB8 RXC  */
	/* PB7  ETXC / PB6 ETXEN / PB5 TXD0 / PB4 TXD1 / PB3 TXD2 / PB2 TXD3 /PB1 EMDCK / PB0 EMDIO*/
	/* PA16 ETCKI / PA20 EMCO / */
	R32_PA_DIR |=  bEMCO;/* PA20��MCO��� */
	R32_PA_DIR &=~ bEMCI;/* PA16��ETCKI���� */
	R32_PB_DIR |=  bMDCK|
#ifndef USE_RMII
			bETHT3|bETHT2|/* ʹ��RMIIʱֻ������������ */
#endif
			bETHT1|bETHT0|bETHTEN|bETHTC;/* RGMII��TXC��TXEN��TXD0/1/2/3��MDCK��Ϊ��� */
	R32_PB_DIR &=~ bETHRC|
#ifndef USE_RMII
			bETHR3|bETHR2|/* ʹ��RMIIʱֻ������������ */
#endif
			bETHR1|bETHR0|bETHRDV|bMDIO;/* RGMII��RXC��RXDV��RXD0/1/2/3��MDIO��Ϊ���� */
}

/*******************************************************************************
 * @fn       RGMII_TXC_Delay
 *
 * @brief    ETH send timing adjustment
 *
 * @param    clock_polarity:send clock polarity
 *           delay_time:delay time(unit : half second)
 *
 * @return   None
 */
void RGMII_TXC_Delay(UINT8 clock_polarity, UINT8 delay_time)
{
	if(clock_polarity)
		ETH->MACCR |= (UINT32)(1<<1);/* ����ʱ�ӷ��� */
	else
		ETH->MACCR &=~(UINT32)(1<<1);/* ����ʱ�ӷ��� */
	if(delay_time<=7)
		ETH->MACCR |= (UINT32)(delay_time<<29);
	else
		printf("Error:delay_time is out of range!\n");
}

/*******************************************************************************
 * @fn       ETH_Mem_Malloc
 *
 * @brief    ETH transceiver queue ram space application
 *
 * @return   Execution status
 */
UINT8 ETH_Mem_Malloc(void)
{
	UINT8* PBUF=ETH_queue_base;/* ��ַӳ�� */
	/* ��Ҫ����һ�������ĵ�ַ�ռ䣬��С���Ϊ((RX_Des_Num+TX_Des_Num)*16 +RX_Des_Num*RX_Buf_Size+TX_Buf_Size*TX_Des_Num)) */
	/* ʵ�ʵ����벽������������ʼʱ����������� */
	memset(PBUF,0,((RX_Des_Num+TX_Des_Num)*16 +RX_Des_Num*RX_Buf_Size+TX_Buf_Size*TX_Des_Num));/* ���� */

	return 0;
}

/*******************************************************************************
 * @fn       ETH_buf_init
 *
 * @brief    ETH transceiver queue initialization
 *
 * @return   Execution status
 */
UINT8 ETH_buf_init(void)
{
#if 0  /*�Ƿ��ڳ�ʼ��ʱ��ӡ�����������������б���׵�ַ  */
	printf("DMA_tx_des:0x%08x\n",pDMATxDscrTab);
	printf("DMA_Rx_des:0x%08x\n",pDMARxDscrTab);
	printf("DMA_Tx_BUF:0x%08x\n",pTx_Buff);
	printf("DMA_rx_BUF:0x%08x\n",pRx_Buff);
#endif
	ETH_MACAddressConfig(ETH_MAC_Address0,local_mac);         /* ���ñ���MAC��ַ */
	ETH_DMATxDescChainInit(pDMATxDscrTab, pTx_Buff,TX_Des_Num);/* ���÷��ͻ������������� */
	ETH_DMARxDescChainInit(pDMARxDscrTab, pRx_Buff,RX_Des_Num);/* ���ý��ջ������������� */
	Globe_RxDes_status.LastRxDes=(ETH_DMADESCTypeDef*)pDMARxDscrTab;/* �������֡��ַ����ֵ */

	return 0;
}

#ifdef USE_USB125M
void myusbssh_init ()	// USBSS host initial
{
	USBSSH->UH_TX_CTRL = 0;
	USBSSH->UH_RX_CTRL = 0;
	USBSSH->LINK_CFG = HP_PENDING | CFG_EQ_EN | DEEMPH_CFG | DOWN_FLAG;// downstream
	USBSSH->LINK_CTRL = POWER_MODE_2 | GO_DISABLED;		// change U3-PHY enter P2

	while( USBSSH->LINK_STATUS & LINK_BUSY ); 			// wait link_busy=0
	printf("power mode: P2 \n");
	USBSSH->USB_CONTROL = HOST_MODE | ITP_EN | INT_BUSY_EN | DMA_EN;// host_mode, release link reset
	USBSSH->UH_TX_CTRL = 0x0;
	USBSSH->USB_STATUS = USB_ACT_FLAG | USB_LMP_TX_FLAG | USB_LMP_RX_FLAG | USB_OV_FLAG;
//	USBSSH->UH_TX_DMA = Usb_Tx_DMAaddr; 					// set tx dma address
//	USBSSH->UH_RX_DMA = Usb_Rx_DMAaddr; 					// set rx dma address
	USBSSH->UEP_CFG = UH_T_EN | UH_R_EN;				// set HOST rx/tx enable
	printf("TERM ENABLE \n");
	USBSS->LINK_CFG |= TERM_EN; // term enable
	USBSS->LINK_INT_CTRL = LINK_DISABLE_IE | LINK_INACT_IE | LINK_RX_DET_IE | TERM_PRESENT_IE | LINK_TXEQ_IE | LINK_RDY_IE;

	USBSS->LINK_CTRL = POWER_MODE_2; // GO RX DETECT
}

/*******************************************************************************
 * @fn      switch_pwr_mode
 *
 * @return  None
 */
void myswitch_pwr_mode(UINT8 pwr_mode)
{
    UINT32 temp;
    temp = USBSS->LINK_CTRL;
    temp &= ~PM_MASK;
    temp |= pwr_mode;
    USBSS->LINK_CTRL = temp;
    while( USBSS->LINK_STATUS & LINK_BUSY );            // wait power mode switch done
}
#endif

/*******************************************************************************
 * @fn      ETH_uClock_enable
 *
 * @brief   ETH peripheral clock initialization
 *
 * @return  None
 */
void ETH_uClock_enable(void)
{
	// config ethernet clock frequency
#ifdef USE_USB125M
	myusbssh_init();
	myswitch_pwr_mode(POWER_MODE_2);
#endif
	R8_SAFE_ACCESS_SIG = 0x57; // enable safe access mode
	R8_SAFE_ACCESS_SIG = 0xa8;

	R8_CLK_MOD_AUX=RB_MCO_EN|    /* ʹ/��MCOƵ����� */
				  RB_SEL_25M;    /* ʹ��25MƵ����� */
#ifdef USE_RMII
	R8_CLK_MOD_AUX = R8_CLK_MOD_AUX & (~RB_SRC_125M_MSK) | RB_INT_125M_EN;
#else
#ifndef USE_USB125M
	R8_CLK_MOD_AUX = (R8_CLK_MOD_AUX&(~RB_SRC_125M_MSK))|RB_EXT_125M_EN;
#else
	R8_CLK_MOD_AUX = (R8_CLK_MOD_AUX&(~RB_SRC_125M_MSK))|RB_INT_125M_EN;
#endif
	/* TXC����Դ�����ڲ����ⲿ��125M��������ʹ��RXC����TXC����ʱ��Ҫ��MACCR[0]Ϊ��  */
#endif
	R8_SAFE_ACCESS_SIG = 0                   /* �˳���ȫģʽ */;
}

/*******************************************************************************
 * @fn        rtl8211dn_Get_Speed
 *
 * @brief     Get PHY link speed
 *
 * @return    link speed
 */
Link_Speed_t rtl8211dn_Get_Speed(void)
{
	Link_Speed_t speed;
	UINT16 val;

	val=ETH_ReadPHYRegister(PHY_ADDRESS,0x11); //�ӵ�31�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ
	switch(val&0xc000)
	{
		case 0xc000:return ETH_disconnect;
		case 0x0000:return ETH_10M;
		case 0x4000:return ETH_100M;
		case 0x8000:return ETH_Gigabit;
		default:break;
	}
	return speed;
}

/*******************************************************************************
 * @fn       ENABLE_ETH_WAKEUP
 *
 * @brief    ENABLE ETH wakeup
 *
 * @return   None
 */
void ENABLE_ETH_WAKEUP(void)
{
	R8_SAFE_ACCESS_SIG = 0x57; // enable safe access mode
	R8_SAFE_ACCESS_SIG = 0xa8;
    R8_SLP_WAKE_CTRL |= RB_SLP_ETH_WAKE;
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
 * @fn          ETH_MACDMA_Config
 *
 * @brief       Mac & DMA config initilization
 *
 * @return      Execution status
 */
UINT8 ETH_MACDMA_Config(void)
{
	unsigned char rval=100;
	ETH_InitTypeDef ETH_InitStructure; 
	
	ETH_uClock_enable();	                    /* ʹ����̫��ʱ�� */
	ETH_SoftwareReset();  						/* ����������� */
	mDelaymS(10);
	while ( (ETH_GetSoftwareResetStatus() == SET)&&(rval >= 0) )
	{
		PRINT("���ڽ�����̫�������λ.\n");
		mDelaymS(10);
		rval--;
		if(rval <= 0)
		{
			printf("��λ��ʱ��\n");				/* ��̫����λ����������125Mʱ�������RXC 125MHzʱ�� */
			printf("*********Please cheak 125MHz clock and Rxc clock!*********\n");
			break;
		}
	}/* �ȴ��������������ɣ������ʵ��λ��ʱ������EMCO�Ƿ������õĲ��������RXC�Ƿ���125MHz��� */
	ETH->MACCR |= ETH_MACCR_RE;
	ETH_StructInit(&ETH_InitStructure); 	 	/* ��ʼ������ΪĬ��ֵ   */
	rval=ETH_Init(&ETH_InitStructure,PHY_ADDRESS);/* ����ETH */
	if(rval==ETH_SUCCESS)//���óɹ�
	{
		ETH_DMAITConfig(ETH_DMA_IT_NIS
						|ETH_DMA_IT_T/* ʹ�ܷ�������ж� */
						|ETH_DMA_IT_R /* ʹ�ܽ�������ж� */
//						|ETH_DMA_IT_TBU,/* ʹ�ܷ����������������ж� */
						,ENABLE );//ʹ����̫�������ж�
	}
	else
	{
		printf("Error:ETH_MACDMA_Config failed!!!\n");
		return ETH_ERROR;
	}
		PFIC_EnableIRQ(ETH_IRQn);
//		printf("ETH_MACDMA_Config finished!\n");
		return rval;
}

/*******************************************************************************
 * @fn       ETH_MACDMA_reConfig
 *
 * @brief    Mac & DMA config reinitilization
 *
 * @return   Execution status
 */
UINT8 ETH_MACDMA_reConfig(void)
{
	unsigned char rval=100;
	ETH_InitTypeDef ETH_InitStructure;

	PFIC_DisableIRQ(ETH_IRQn);
	ETH->DMABMR |= ETH_DMABMR_SR;						/* ����������� */
	mDelaymS(10);
	while ( (ETH_GetSoftwareResetStatus() == SET)&&(rval >= 0) )
	{
		PRINT("���ڽ�����̫���������.\n");
		mDelaymS(10);
		rval--;
		if(rval <= 0)
		{
			printf("��λ��ʱ��\n");				/* ��̫����λ����������125Mʱ�������RXC 125MHzʱ�� */
			printf("*********Please cheak 125MHz clock and Rxc clock!*********\n");
			break;
		}
	}/* �ȴ��������������ɣ������ʵ��λ��ʱ������EMCO�Ƿ������õĲ��������RXC�Ƿ���125MHz��� */
	ETH->MACCR |= ETH_MACCR_RE;
	ETH_StructInit(&ETH_InitStructure); 	 	/* ��ʼ������ΪĬ��ֵ   */
	rval=ETH_Init(&ETH_InitStructure,PHY_ADDRESS);/* ����ETH */
	if(rval==ETH_SUCCESS)//���óɹ�
	{
		ETH_DMAITConfig(ETH_DMA_IT_NIS
						|ETH_DMA_IT_T/* ʹ�ܷ�������ж� */
						|ETH_DMA_IT_R /* ʹ�ܽ�������ж� */
//						|ETH_DMA_IT_TBU,/* ʹ�ܷ����������������ж� */
						,ENABLE );//ʹ����̫�������ж�
	}
	else
	{
		printf("Error:ETH_MACDMA_Config failed!!!\n");
		return ETH_ERROR;
	}
		PFIC_EnableIRQ(ETH_IRQn);
//		printf("ETH_MACDMA_reConfig finished!\n");

		ETH_buf_init();                /*����������������ʼ��*/

		ETH->DMAOMR |= ETH_DMAOMR_SR;  /* ��������DMA */
		ETH->DMAOMR |= ETH_DMAOMR_TSF; /* �����洢ת��,������Ӳ����д����͵�ʱ�򣬱��뿪����һ����Ĭ�Ͽ���Ӳ����дУ��� */
		ETH->DMAOMR |= ETH_DMAOMR_ST;  /* ��DMA����  */
		mDelaymS(2);
		ETH->MACCR |= ETH_MACCR_TE;/* ����Mac����ʹ�� */

		Check_TxDes();/* ������������� */

		return rval;
}

/*******************************************************************************
 * @fn         Mac_init
 *
 * @brief      Eth phripheral initilization
 *
 * @return     Execution status
 */
UINT8 Mac_init(void)
{
	ETH_GPIO_Init();/* GPIO��ʼ�� */
	if( ETH_Mem_Malloc() )/* ��̫�������������������������ڴ�ĺ��� */
	{
		return 1;
	}
	if( ETH_MACDMA_Config()!=1 )   /* MAC�ļĴ�����ʼ�� */
	{
		return 2;
	}
	ETH_buf_init();                /*����������������ʼ��*/

	ETH->DMAOMR |= ETH_DMAOMR_SR;  /* ��������DMA */
	ETH->DMAOMR |= ETH_DMAOMR_TSF; /* �����洢ת��,������Ӳ����д����͵�ʱ�򣬱��뿪����һ����Ĭ�Ͽ���Ӳ����дУ��� */
	ETH->DMAOMR |= ETH_DMAOMR_ST;  /* ��DMA����  */
	mDelaymS(2);
	ETH->MACCR |= ETH_MACCR_TE;/* ����Mac����ʹ�� */
	return 0;
}

/*******************************************************************************
 * @fn        ETH_init
 *
 * @brief     Eth initilization
 *
 * @return    None
 */
void ETH_init(void)
{
	UINT8 rc;
	rc=Mac_init();
	while(rc)
	{
		PRINT("MAC init failed.retrying...\n");
		mDelaymS(2);
		rc=Mac_init();
	}
}

/*******************************************************************************
 * @fn        ENABLE_PMT_INT
 *
 * @brief     Eth power manager interrupt initilization
 *
 * @return    None
 */
void ENABLE_PMT_INT(void)
{
	PFIC_EnableIRQ(PMT_IRQn);
}

/*******************************************************************************
 * @fn        Check_TxDes
 *
 * @brief     Check Transmit descriptor
 *
 * @return    None
 */
void Check_TxDes(void)
{
	if(DMATxDescToSet == (ETH_DMADESCTypeDef*)ETH->DMACHTDR)
		return;
	ETH_DMATxDescChainInit(pDMATxDscrTab, pTx_Buff,TX_Des_Num);/* ���÷��ͻ������������� */
	DMATxDescToSet = (ETH_DMADESCTypeDef*)ETH->DMACHTDR;
}

/*******************************************************************************
 * @fn        ETH_IRQHandler
 *
 * @brief     Eth interrupt
 *
 * @return    None
 */
void ETH_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void ETH_IRQHandler(void)
{
#if 0 /* ������һ�����Լӿ��жϺ�����ɵ��ٶ� */
	if(ETH->DMASR&ETH_DMA_IT_TBU)/* �з��������������� */
	{
		ETH_DMAClearITPendingBit(ETH_DMA_IT_TBU);
	}
#endif
	/*-----------------��������ж�----------------------*/
	if(ETH->DMASR&ETH_DMA_IT_T)/* ��һ֡�Ѿ�������� */
	{
		ETH_DMAClearITPendingBit(ETH_DMA_IT_T);
//		printf("t\n");
		enable_send|=0x01;/* �������� */
		rece_timeout_cnt=0;
	}
	/*-----------------��������ж�----------------------*/
	if(ETH->DMASR&ETH_DMA_IT_R)/* ��һ֡�Ѿ�������� */
	{
		while((DMARxDescToGet->Status&ETH_DMARxDesc_OWN)==0)/*�ж���������û�ж���*/
		{
			ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
			DMARxDescToGet->Status|=ETH_DMARxDesc_OWN;                /*  */
			DMARxDescToGet=(ETH_DMADESCTypeDef*)(DMARxDescToGet->Buffer2NextDescAddr);
			Globe_RxDes_status.pengding_RxDes_cnt++;
		}
		/* �����ѭ���Ķ�ȡ��̫��֡�����ٶȲ����죬����������ζ�ȡ�ļ�϶����̫������֡����������
		 * ������������RX_Des_Num��20200704��Ϊ8���������Σ����Ϊ��֤��������������ת��Ӧ����֮
		 * ǰ���յ��İ� */
		if(Globe_RxDes_status.pengding_RxDes_cnt>RX_Des_Num)
		{
			Globe_RxDes_status.pengding_RxDes_cnt=1;
			Globe_RxDes_status.package_loss_cnt++;
		}
//		printf("r\n");
		enable_send|=0x02;/* �������� */
		rece_timeout_cnt=0;
	}
	/* ���DMASR��־λ */
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}

/*******************************************************************************
 * @fn      PMT_IRQHandler
 *
 * @brief   Eth power manager interrupt
 *
 * @return  None
 */
void PMT_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void PMT_IRQHandler(void)
{
	printf("ETH->MACPMTCSR:0x%08x\n",ETH->MACPMTCSR);
	printf("PMT_IRQHandler\n");
}

/*******************************************************************************
 * @fn           mac_send
 *
 * @brief        Eth send
 *
 * @param        length:length of data will be sent
 *
 * @return       Execution status
 */
UINT8 mac_send(uint32_t length)
{
	UINT8 rc;

	if((DMATxDescToSet->Status&ETH_DMATxDesc_OWN)!=(UINT32)RESET )/* �����ϲ�Ӧ�ý�������������������Ҫ��鷢�������� */
	{
		/* ���������ԭ��һ������û�б���������жϵ�����¾ͼ���ʹ�ø÷���������  */
		printf("Error:DMATxDescToSet can no t use!\n");
		printf("send:DMACHTDR:x%08x\n",ETH->DMACHTDR);
		printf("send:DMATxDescToSet:0x%08x\n",DMATxDescToSet);
//		printf("send:0x%08x:0x%08x\n",&(DMATxDescToSet->Status),DMATxDescToSet->Status);
//		printf("send:0x%08x:0x%08x\n",&(DMATxDescToSet->ControlBufferSize),DMATxDescToSet->ControlBufferSize);
//		printf("send:0x%08x:0x%08x\n",&(DMATxDescToSet->Buffer1Addr),DMATxDescToSet->Buffer1Addr);
//		printf("send:0x%08x:0x%08x\n",&(DMATxDescToSet->Buffer2NextDescAddr),DMATxDescToSet->Buffer2NextDescAddr);
		printf("send:DMASR:08%08x\n",ETH->DMASR);
		return 1;    /* ����,OWNλ�������� */
	}
	DMATxDescToSet->ControlBufferSize=(length&ETH_DMATxDesc_TBS1);        //����֡����,bits[12:0]
	DMATxDescToSet->Status=ETH_DMATxDesc_LS|ETH_DMATxDesc_FS|ETH_DMATxDesc_IC|ETH_DMATxDesc_CIC_TCPUDPICMP_Full;//�������һ���͵�һ��λ����λ(1������������һ֡)�����õڶ���������ָ����һ��������
//	DMATxDescToSet->Status|=ETH_DMATxDesc_DC;                              /* ���Զ����㲢���CRC */
	DMATxDescToSet->Status|=ETH_DMATxDesc_OWN;                             //����Tx��������OWNλ,buffer�ع�ETH DMA

	/* ��ǰ��������������ת���ƣ���Ϊ���֣�һ��ֱ��ȡ���ĸ�˫�ֵ�ֵ�����ַ�����Ȼ�죬���������CHTDR�ܷɵ�����£���ȫ�̷ɵ��ķ���
	  * ���Ǽ���Ƿ������һ����������������ֱ�������4��˫�ֵ�λ�á�CH565/569���������б�����ʽ�ͻ�ʽ���ϵķ����Ų��� */
#if 0
	DMATxDescToSet==(ETH_DMADESCTypeDef*)DMATxDescToSet->Buffer2NextDescAddr;
#else
	if(DMATxDescToSet>=((ETH_DMADESCTypeDef*)pDMATxDscrTab+(TX_Des_Num-1)))
		DMATxDescToSet = (ETH_DMADESCTypeDef*)(pDMATxDscrTab);
	else
		DMATxDescToSet++;
#endif
	ETH->DMASR=ETH_DMASR_TBUS;                                         /* ����ETH DMA TBUSλ */
	ETH->DMATPDR=0;                                                    /* �ָ�DMA���� */

	return 0;
}

/*******************************************************************************
 * @fn        mac_rece
 *
 * @brief     Eth receive
 *
 * @param     ptr:pointer to the receive buffer
 *
 * @return    The length of received data
 */
UINT16 mac_rece(UINT8 **ptr)
{

	UINT16 frame_length;
	if(Globe_RxDes_status.pengding_RxDes_cnt==0)
	{
		frame_length=0;
		printf("Error:no package received!\n");
	}
	else
	{
		*ptr=(UINT8*)(Globe_RxDes_status.LastRxDes->Buffer1Addr);
		frame_length=((UINT16)(((Globe_RxDes_status.LastRxDes->Status)&0x3fff0000)>>16));
		(Globe_RxDes_status.LastRxDes) = (ETH_DMADESCTypeDef*)(Globe_RxDes_status.LastRxDes->Buffer2NextDescAddr);
		Globe_RxDes_status.pengding_RxDes_cnt--;
	}

	return frame_length;
}

/*******************************************************************************
 * @fn        ETH_StructInit
 *
 * @brief     Eth config struct initialization
 *
 * @param     ETH_InitStruct:pointer to the eth config struct
 *
 * @return    None
 */
void ETH_StructInit(ETH_InitTypeDef *ETH_InitStruct)
{
	  /*------------------------------------------   MACĬ�ϳ�ʼ������  ---------------------------------------------*/

	  /* ʹ��ʹ��������Զ�Э��ģʽ */
	  ETH_InitStruct->ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	  /* ʹ��MAC���Ź� */
	  ETH_InitStruct->ETH_Watchdog = ETH_Watchdog_Enable;
	  /* ʹ��Jabber�ڰ�˫��ģʽ�У������壬CH56X��֧�ְ�˫�� */
	  ETH_InitStruct->ETH_Jabber = ETH_Jabber_Enable;
	  /* ����֡���Ϊ96bit */
	  ETH_InitStruct->ETH_InterFrameGap = ETH_InterFrameGap_96Bit;
	  /* ��˫��ģʽ��ʹ���ز��������ܣ� */
	  ETH_InitStruct->ETH_CarrierSense = ETH_CarrierSense_Enable;
	  /* PHY���ٶ�Ĭ����ΪΪ1000Mbps */
	  ETH_InitStruct->ETH_Speed = ETH_Speed_1000M;
	  /* ��˫��ģʽ��������� own frame,�����壬CH56X��֧�ְ�˫�� */
	  ETH_InitStruct->ETH_ReceiveOwn = ETH_ReceiveOwn_Enable;
	  /* �ر�MII�ӿڵķ�������,�����壬CH56X��֧�ְ�˫��  */
	  ETH_InitStruct->ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	  /* ʹ��ȫ˫��ģʽ,�����壬CH56X��֧�ְ�˫�� */
	  ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
	  /* Ĭ��ipv4��TCP/UDP/ICMP��֡У���ж�� */
	  ETH_InitStruct->ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
	  /* ������˫��ģʽ�µ����Դ��书�ܣ������壬CH56X��֧�ְ�˫�� */
	  ETH_InitStruct->ETH_RetryTransmission = ETH_RetryTransmission_Enable;
	  /* Ĭ���Զ�ȥ��PDA/CRC����*/
	  ETH_InitStruct->ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Enable;
	  /* ���ð�˫��ģʽ�µ�����ش������¼�10 slot�������壬CH56X��֧�ְ�˫�� */
	  ETH_InitStruct->ETH_BackOffLimit = ETH_BackOffLimit_10;
	  /* �رհ�˫��ģʽ�µ���ʱ��鹦�ܣ������壬CH56X��֧�ְ�˫��*/
	  ETH_InitStruct->ETH_DeferralCheck = ETH_DeferralCheck_Disable;
	  /* Ĭ�Ͻ�������֡ */
	  ETH_InitStruct->ETH_ReceiveAll = ETH_ReceiveAll_Enable;
	  /* �ر�MAC��ַ��Դ��ַ���˹��� */
	  ETH_InitStruct->ETH_SourceAddrFilter = ETH_SourceAddrFilter_Disable;
	  /* Do not forward control frames that do not pass the address filtering */
	  ETH_InitStruct->ETH_PassControlFrames = ETH_PassControlFrames_BlockAll;
	  /* ��ֹ�������еĹ㲥֡ */
	  ETH_InitStruct->ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Disable;
	  /* ������Զ�˵�ַ���� */
	  ETH_InitStruct->ETH_DestinationAddrFilter = ETH_DestinationAddrFilter_Normal;
	  /* �رջ��ģʽ�ĵ�ַ���� */
	  ETH_InitStruct->ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	  /* �����鲥��ַʹ��������ַ���� */
	  ETH_InitStruct->ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	  /* �Ե�����ַʹ��������ַ���� */
	  ETH_InitStruct->ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
	  /* ��ʼ��HASH��ĸ�λ�Ĵ��� */
	  ETH_InitStruct->ETH_HashTableHigh = 0x0;
	  /* ��ʼ��HASH��ĵ�λλ�Ĵ��� */
	  ETH_InitStruct->ETH_HashTableLow = 0x0;
	  /* ��������*/
	  ETH_InitStruct->ETH_PauseTime = 0x0;
	  ETH_InitStruct->ETH_ZeroQuantaPause = ETH_ZeroQuantaPause_Disable;
	  ETH_InitStruct->ETH_PauseLowThreshold = ETH_PauseLowThreshold_Minus4;
	  ETH_InitStruct->ETH_UnicastPauseFrameDetect = ETH_UnicastPauseFrameDetect_Disable;
	  ETH_InitStruct->ETH_ReceiveFlowControl = ETH_ReceiveFlowControl_Disable;
	  ETH_InitStruct->ETH_TransmitFlowControl = ETH_TransmitFlowControl_Disable;
	  /* VLANtag config (VLAN field not checked) */
	  ETH_InitStruct->ETH_VLANTagComparison = ETH_VLANTagComparison_16Bit;
	  ETH_InitStruct->ETH_VLANTagIdentifier = 0x0;

	  /*---------------------- DMA ��ʼ��        -------------------------------*/

	  /* �رն���TCP/IP����֡ */
	  ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Disable;
	  /* �����������ݵĴ洢ת������ */
	  ETH_InitStruct->ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
	  /* Flush received frame that created FIFO overflow */
	  ETH_InitStruct->ETH_FlushReceivedFrame = ETH_FlushReceivedFrame_Enable;
	  /* ��������ģʽ�Ĵ洢ת������ */
	  ETH_InitStruct->ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
	  /* ������ֵģʽ�µķ���FIFO����ֵΪ64�ֽ� */
	  ETH_InitStruct->ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_64Bytes;
	  /* ��ֹת������֡ */
	  ETH_InitStruct->ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
	  /* ��ת����С�ĺ�֡ */
	  ETH_InitStruct->ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	  /* ����ֱͨģʽ�µķ���FIFO��ֵΪ64�ֽ� */
	  ETH_InitStruct->ETH_ReceiveThresholdControl = ETH_ReceiveThresholdControl_64Bytes;
	  /* �رմ���ڶ�֡���� */
	  ETH_InitStruct->ETH_SecondFrameOperate = ETH_SecondFrameOperate_Disable;
	  /* ����DMA����ĵ�ַ���빦�� */
	  ETH_InitStruct->ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
	  /* �����̶�ͻ������ */
	  ETH_InitStruct->ETH_FixedBurst = ETH_FixedBurst_Enable;
	  /* DMA���͵����ͻ������Ϊ32 */
	  ETH_InitStruct->ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
	  /* DMA���յ����ͻ������Ϊ32 */
	  ETH_InitStruct->ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
	  /* DMA Ring mode skip length = 0 */
	  ETH_InitStruct->ETH_DescriptorSkipLength = 0x0;
	  /* Equal priority (round-robin) between transmit and receive DMA engines */
	  ETH_InitStruct->ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_1_1;

	  /* ���Ի����� */
	  ETH_InitStruct->ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;   		//������������Ӧ����
	  ETH_InitStruct->ETH_LoopbackMode = ETH_LoopbackMode_Disable;				//�رշ���
	  ETH_InitStruct->ETH_RetryTransmission = ETH_RetryTransmission_Disable; 		//�ر��ش�����kp
	  ETH_InitStruct->ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable; 	//�ر��Զ�ȥ��PDA/CRC����
	  ETH_InitStruct->ETH_ReceiveAll = ETH_ReceiveAll_Enable;						       //�رս������е�֡
	  ETH_InitStruct->ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;//����������й㲥֡
	  ETH_InitStruct->ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;			//�رջ��ģʽ�ĵ�ַ����
	  ETH_InitStruct->ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;//�����鲥��ַʹ��������ַ����
	  ETH_InitStruct->ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;	//�Ե�����ַʹ��������ַ����
	#ifdef CHECKSUM_BY_HARDWARE
		ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable; 			//����ipv4��TCP/UDP/ICMP��֡У���ж��
	#endif
		//������ʹ��֡У���ж�ع��ܵ�ʱ��һ��Ҫʹ�ܴ洢ת��ģʽ,�洢ת��ģʽ��Ҫ��֤����֡�洢��FIFO��,
		//����MAC�ܲ���/ʶ���֡У��ֵ,����У����ȷ��ʱ��DMA�Ϳ��Դ���֡,����Ͷ�������֡
		ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; //��������TCP/IP����֡
		ETH_InitStruct->ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;     //�����������ݵĴ洢ת��ģʽ
		ETH_InitStruct->ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;   //�����������ݵĴ洢ת��ģʽ
		ETH_InitStruct->ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Enable;     	//��ֹת������֡
		ETH_InitStruct->ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Enable;	//��ת����С�ĺ�֡
		ETH_InitStruct->ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;  		//�򿪴���ڶ�֡����
		ETH_InitStruct->ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;  	//����DMA����ĵ�ַ���빦��
		ETH_InitStruct->ETH_FixedBurst = ETH_FixedBurst_Enable;            			//�����̶�ͻ������
		ETH_InitStruct->ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;     		//DMA���͵����ͻ������Ϊ32������
		ETH_InitStruct->ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;			//DMA���յ����ͻ������Ϊ32������
		ETH_InitStruct->ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
}

/*******************************************************************************
 * @fn          ETH_Init
 *
 * @brief       Eth register & PHY register initialization
 *
 * @param       ETH_InitStruct:pointer to the eth config struct
 *              PHYAddress:PHY address
 *
 * @return      Execution status
 */
UINT32 ETH_Init(ETH_InitTypeDef *ETH_InitStruct, UINT16 PHYAddress)
{
  UINT32  tmpreg = 0;
  UINT16 RegValue = 0;
  __IO UINT32 i = 0;
  UINT32 hclk = 80000000;
  __IO UINT32 timeout = 0;
  /* MAC --------------------------*/

  /*-------------------------------- MAC Config ------------------------------*/
  /*---------------------- ETHERNET MACMIIAR Configuration -------------------*/
  /* Get the ETHERNET MACMIIAR value */
  tmpreg = ETH->MACMIIAR;
  /* Clear CSR Clock Range CR[2:0] bits */
  tmpreg &= MACMIIAR_CR_MASK;
  /* Get hclk frequency value */

/*__________ע��___________ע��______ע��_______ע��______ע��_______ע��_______ע��_____ע��______*/
  /* �޸�ϵͳ��Ƶ��Ҫͬ���޸����ֵ */
  hclk=12000000;//�õ�ϵͳʱ��Ƶ��(��λ:Hz)
  /* �����ȡϵͳ��Ƶ��ԭ����Ҫ��Ϊ��ȷ��SMI�ӿ�ʱ���ߵķ�Ƶ�������о�Ӱ�첻���Ҿ���Ӧ��SMIʱ�Ӿ����ܽ��͵㡣 */
//    tmpreg |= (UINT32)ETH_MACMIIAR_CR_Div42;/* �͹̶�ѡ��42��Ƶ���ˣ�80Mʱ�ӱ�42��Ƶ��Ƶ�ʾͺܵ��� */
    tmpreg |= (UINT32)ETH_MACMIIAR_CR_Div62;
//    tmpreg |= (UINT32)ETH_MACMIIAR_CR_Div102;
  /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
  ETH->MACMIIAR = (UINT32)tmpreg;
  /*-------------------- PHY initialization and configuration ----------------*/
  /* Put the PHY in reset mode */
  if(!(ETH_WritePHYRegister(PHYAddress, PHY_BCR, PHY_Reset)))
  {
    /* Return ERROR in case of write timeout */
	  printf("����㸴λʧ�ܣ�\n");
      return 0;
  }
  else
  {
//	  PRINT("����㸴λ�ɹ���\n");
  }

  /* Delay to assure PHY reset */
	mDelaymS(200);

  if(ETH_InitStruct->ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
  {
#ifndef FIBER_1000M
    /* We wait for linked status... */
//    do
//    {
//      timeout++;
//      RegValue=ETH_ReadPHYRegister(PHYAddress, PHY_BSR) ;
////      printf("RegValue:0x%04x\n",RegValue);
//    } while (!(RegValue & PHY_Linked_Status) && (timeout < PHY_READ_TO));
//
//    /* Return ERROR in case of timeout */
//    if(timeout >= PHY_READ_TO)
//    {
//    	printf("READ PHY_BSR TIMEOUT!\n");
//    	return ETH_ERROR;
//    }

    /* Reset Timeout counter */
    timeout = 0;
    /* Enable Auto-Negotiation */
    if(!(ETH_WritePHYRegister(PHYAddress, PHY_BCR, PHY_AutoNegotiation)))
    {
      /* Return ERROR in case of write timeout */
    	printf("WRITE PHY_AutoNegotiation  TIMEOUT!\n");
    	return ETH_ERROR;
    }

    /* Wait until the auto-negotiation will be completed */
    do
    {
      timeout++;
    } while (!(ETH_ReadPHYRegister(PHYAddress, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (UINT32)PHY_READ_TO));

    /* Return ERROR in case of timeout */
    if(timeout == PHY_READ_TO)
    {
    	PRINT("AutoNego timeout!\n");
    	return ETH_ERROR;
    }

    /* Reset Timeout counter */
    timeout = 0;
#endif
    /* Read the result of the auto-negotiation */
    timeout=100;
    do
    {
    	RegValue = ETH_ReadPHYRegister(PHYAddress, PHY_BMSR);
    	mDelaymS(2);/* �ȴ������� */
        timeout--;
        if(timeout<=0)
        {
        	printf("Wait phy linking timeout!\n");
        	break;
        }
    }
    while((RegValue & 0x0004)==0);
    if(RegValue & 0x0004)
    {
    	PRINT("PHY UTP/fiber linked.PHY_BMSR:0x%04x\n",RegValue);
#ifdef USE_ETH_PHY_INTERRUPT
    	globe_eth_status.phy_link_status=link_ok;
#endif
    }
#ifdef USE_RTL8211FS
#ifndef FIBER_1000M
    ETH_WritePHYRegister(PHYAddress, 31,0x0a43 );
    RegValue = ETH_ReadPHYRegister(PHYAddress, 26);
    PRINT("PHY_SR:0x%04x\n",RegValue);
    if( RegValue & 0x0008 )
    {
    	ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
    	PRINT("full duplex.\n");
#ifdef USE_ETH_PHY_INTERRUPT
    	globe_eth_status.phy_link_duples=full_duples;
#endif
    }
    else
    {
    	ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
    	PRINT("half duplex!\n");
#ifdef USE_ETH_PHY_INTERRUPT
    	globe_eth_status.phy_link_duples=half_duples;
#endif
    }
    if(( RegValue & 0x0030 ) == 0x0000)
    {
    	ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
    	PRINT("Link speed:10Mbps.\n");
#ifdef USE_ETH_PHY_INTERRUPT
    	globe_eth_status.phy_link_speed=linked_10M;
#endif
    }
    else if(( RegValue & 0x0030 ) == 0x0010)
    {
    	ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
    	PRINT("Link speed:100Mbps.\n");
#ifdef USE_ETH_PHY_INTERRUPT
    	globe_eth_status.phy_link_speed=linked_100M;
#endif
    }
    else if(( RegValue & 0x0030 ) == 0x0020)
    {
    	ETH_InitStruct->ETH_Speed = ETH_Speed_1000M;
    	PRINT("Link speed:1000Mbps.\n");
#ifdef USE_ETH_PHY_INTERRUPT
    	globe_eth_status.phy_link_speed=linked_1000M;
#endif
    }
    else
    {
    	printf("Do not support peer device.\nPlease link with 1000Base_T/100Base-TX/10Base-T device and then restart.\n");
    	while(1);
    }
    /* ʹ��RTL8211FS��TXC�ڲ���ʱ1.5���� */
    ETH_WritePHYRegister(PHYAddress, 31,0x0d08);
    RegValue = ETH_ReadPHYRegister(PHYAddress, 17);
//    printf("page d08 reg 17 is %04x\n",RegValue);
    ETH_WritePHYRegister(PHYAddress, 17,RegValue | 0x0100);
    RegValue = ETH_ReadPHYRegister(PHYAddress, 17);
//    printf("page d08 reg 17 is %04x\n",RegValue);

//    ETH_WritePHYRegister(PHYAddress, 31,0xd08 );
//    RegValue = ETH_ReadPHYRegister(PHYAddress, 0x10);
//    PRINT("SGMII ANAR:0x%04x\n",RegValue);
#else/* fiberģʽ�£��ֶ�ָ��MAC�ٶ� */
    ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStruct->ETH_Speed = ETH_Speed_1000M;
#endif
#endif
#ifdef USE_RTL8211DN
#ifndef FIBER_1000M
    RegValue = ETH_ReadPHYRegister(PHYAddress, 0x11);
    PRINT("PHY_SR:0x%04x\n",RegValue);
    if( RegValue & 0x2000 )
    {
    	ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
    	PRINT("full duplex.\n");
    }
    else
    {
    	ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
    	PRINT("half duplex!\n");
    }
    if(( RegValue & 0xC000 ) == 0x0000)
    {
    	ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
    	PRINT("Link speed:10Mbps.\n");
    }
    else if(( RegValue & 0xC000 ) == 0x4000)
    {
    	ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
    	PRINT("Link speed:100Mbps.\n");
    }
    else if(( RegValue & 0xC000 ) == 0x8000)
    {
    	ETH_InitStruct->ETH_Speed = ETH_Speed_1000M;
    	PRINT("Link speed:1000Mbps.\n");
    }
    else
    {
    	printf("Do not support peer device.\nPlease link with 1000Base_T/100Base-TX/10Base-T device and then restart.\n");
    	while(1);
    }

#else
    /* fiberģʽ�£��ֶ�ָ��MAC�ٶ� */
    ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStruct->ETH_Speed = ETH_Speed_1000M;
#endif
#endif

  }
  /*------------------------ ETHERNET MACCR Configuration --------------------*/
  /* Get the ETHERNET MACCR value */
  tmpreg = ETH->MACCR;
  /* Clear WD, PCE, PS, TE and RE bits */
  tmpreg &= MACCR_CLEAR_MASK;
  /* Set the WD bit according to ETH_Watchdog value */
  /* Set the JD: bit according to ETH_Jabber value */
  /* Set the IFG bit according to ETH_InterFrameGap value */
  /* Set the DCRS bit according to ETH_CarrierSense value */
  /* Set the FES bit according to ETH_Speed value */
  /* Set the DO bit according to ETH_ReceiveOwn value */
  /* Set the LM bit according to ETH_LoopbackMode value */
  /* Set the DM bit according to ETH_Mode value */
  /* Set the IPCO bit according to ETH_ChecksumOffload value */
  /* Set the DR bit according to ETH_RetryTransmission value */
  /* Set the ACS bit according to ETH_AutomaticPadCRCStrip value */
  /* Set the BL bit according to ETH_BackOffLimit value */
  /* Set the DC bit according to ETH_DeferralCheck value */
  tmpreg |= (UINT32)(ETH_InitStruct->ETH_Watchdog |
                  ETH_InitStruct->ETH_Jabber |
                  ETH_InitStruct->ETH_InterFrameGap |
                  ETH_InitStruct->ETH_CarrierSense |
                  ETH_InitStruct->ETH_Speed |
                  ETH_InitStruct->ETH_ReceiveOwn |
                  ETH_InitStruct->ETH_LoopbackMode |
                  ETH_InitStruct->ETH_Mode |
                  ETH_InitStruct->ETH_ChecksumOffload |
                  ETH_InitStruct->ETH_RetryTransmission |
                  ETH_InitStruct->ETH_AutomaticPadCRCStrip |
                  ETH_InitStruct->ETH_BackOffLimit |
                  ETH_InitStruct->ETH_DeferralCheck);
  /* Write to ETHERNET MACCR */
  ETH->MACCR = (UINT32)tmpreg;
  ETH->MACCR|=(1<<16);/* �ر��ز����� */
  /*----------------------- ETHERNET MACFFR Configuration --------------------*/
  /* Set the RA bit according to ETH_ReceiveAll value */
  /* Set the SAF and SAIF bits according to ETH_SourceAddrFilter value */
  /* Set the PCF bit according to ETH_PassControlFrames value */
  /* Set the DBF bit according to ETH_BroadcastFramesReception value */
  /* Set the DAIF bit according to ETH_DestinationAddrFilter value */
  /* Set the PR bit according to ETH_PromiscuousMode value */
  /* Set the PM, HMC and HPF bits according to ETH_MulticastFramesFilter value */
  /* Set the HUC and HPF bits according to ETH_UnicastFramesFilter value */
  /* Write to ETHERNET MACFFR */
  ETH->MACFFR = (UINT32)(ETH_InitStruct->ETH_ReceiveAll |
                          ETH_InitStruct->ETH_SourceAddrFilter |
                          ETH_InitStruct->ETH_PassControlFrames |
                          ETH_InitStruct->ETH_BroadcastFramesReception |
                          ETH_InitStruct->ETH_DestinationAddrFilter |
                          ETH_InitStruct->ETH_PromiscuousMode |
                          ETH_InitStruct->ETH_MulticastFramesFilter |
                          ETH_InitStruct->ETH_UnicastFramesFilter);
  /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
  /* Write to ETHERNET MACHTHR */
  ETH->MACHTHR = (UINT32)ETH_InitStruct->ETH_HashTableHigh;
  /* Write to ETHERNET MACHTLR */
  ETH->MACHTLR = (UINT32)ETH_InitStruct->ETH_HashTableLow;
  /*----------------------- ETHERNET MACFCR Configuration --------------------*/
  /* Get the ETHERNET MACFCR value */
  tmpreg = ETH->MACFCR;
  /* Clear xx bits */
  tmpreg &= MACFCR_CLEAR_MASK;

  /* Set the PT bit according to ETH_PauseTime value */
  /* Set the DZPQ bit according to ETH_ZeroQuantaPause value */
  /* Set the PLT bit according to ETH_PauseLowThreshold value */
  /* Set the UP bit according to ETH_UnicastPauseFrameDetect value */
  /* Set the RFE bit according to ETH_ReceiveFlowControl value */
  /* Set the TFE bit according to ETH_TransmitFlowControl value */
  tmpreg |= (UINT32)((ETH_InitStruct->ETH_PauseTime << 16) |
                   ETH_InitStruct->ETH_ZeroQuantaPause |
                   ETH_InitStruct->ETH_PauseLowThreshold |
                   ETH_InitStruct->ETH_UnicastPauseFrameDetect |
                   ETH_InitStruct->ETH_ReceiveFlowControl |
                   ETH_InitStruct->ETH_TransmitFlowControl);
  /* Write to ETHERNET MACFCR */
  ETH->MACFCR = (UINT32)tmpreg;
  /*----------------------- ETHERNET MACVLANTR Configuration -----------------*/
  /* Set the ETV bit according to ETH_VLANTagComparison value */
  /* Set the VL bit according to ETH_VLANTagIdentifier value */
  ETH->MACVLANTR = (UINT32)(ETH_InitStruct->ETH_VLANTagComparison |
                             ETH_InitStruct->ETH_VLANTagIdentifier);

  /*-------------------------------- DMA Config ------------------------------*/
  /*----------------------- ETHERNET DMAOMR Configuration --------------------*/
  /* Get the ETHERNET DMAOMR value */
  tmpreg = ETH->DMAOMR;
  /* Clear xx bits */
//  tmpreg &= DMAOMR_CLEAR_MASK;

  /* Set the DT bit according to ETH_DropTCPIPChecksumErrorFrame value */
  /* Set the RSF bit according to ETH_ReceiveStoreForward value */
  /* Set the DFF bit according to ETH_FlushReceivedFrame value */
  /* Set the TSF bit according to ETH_TransmitStoreForward value */
  /* Set the TTC bit according to ETH_TransmitThresholdControl value */
  /* Set the FEF bit according to ETH_ForwardErrorFrames value */
  /* Set the FUF bit according to ETH_ForwardUndersizedGoodFrames value */
  /* Set the RTC bit according to ETH_ReceiveThresholdControl value */
  /* Set the OSF bit according to ETH_SecondFrameOperate value */
  tmpreg |= (UINT32)(ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame |
                  ETH_InitStruct->ETH_ReceiveStoreForward |
                  ETH_InitStruct->ETH_FlushReceivedFrame |
                  ETH_InitStruct->ETH_TransmitStoreForward |
                  ETH_InitStruct->ETH_TransmitThresholdControl |
                  ETH_InitStruct->ETH_ForwardErrorFrames |
                  ETH_InitStruct->ETH_ForwardUndersizedGoodFrames |
                  ETH_InitStruct->ETH_ReceiveThresholdControl |
                  ETH_InitStruct->ETH_SecondFrameOperate);
  /* Write to ETHERNET DMAOMR */
  ETH->DMAOMR = (UINT32)tmpreg;

  /*----------------------- ETHERNET DMABMR Configuration --------------------*/
  /* Set the AAL bit according to ETH_AddressAlignedBeats value */
  /* Set the FB bit according to ETH_FixedBurst value */
  /* Set the RPBL and 4*PBL bits according to ETH_RxDMABurstLength value */
  /* Set the PBL and 4*PBL bits according to ETH_TxDMABurstLength value */
  /* Set the DSL bit according to ETH_DesciptorSkipLength value */
  /* Set the PR and DA bits according to ETH_DMAArbitration value */
  ETH->DMABMR = (UINT32)(ETH_InitStruct->ETH_AddressAlignedBeats |
                          ETH_InitStruct->ETH_FixedBurst |
                          ETH_InitStruct->ETH_RxDMABurstLength | /* !! if 4xPBL is selected for Tx or Rx it is applied for the other */
                          ETH_InitStruct->ETH_TxDMABurstLength |
                         (ETH_InitStruct->ETH_DescriptorSkipLength << 2) |
                          ETH_InitStruct->ETH_DMAArbitration |
                          ETH_DMABMR_USP); /* Enable use of separate PBL for Rx and Tx */

  #ifdef USE_ENHANCED_DMA_DESCRIPTORS
    /* Enable the Enhanced DMA descriptors */
    ETH->DMABMR |= ETH_DMABMR_EDE;
  #endif /* USE_ENHANCED_DMA_DESCRIPTORS */

  /* Return Ethernet configuration success */
  return ETH_SUCCESS;
}

/*******************************************************************************
 * @fn         ETH_MACAddressConfig
 *
 * @brief     Mac address initialization
 *
 * @param     MacAddr - Mac address register
 *            PHYAddress - pointer to the Mac address string
 *
 * @return     None
 */
void ETH_MACAddressConfig(UINT32 MacAddr, UINT8 *Addr)
{
	UINT32 tmpreg;

  /* Calculate the selected MAC address high register */
  tmpreg = ((UINT32)Addr[5] << 8) | (UINT32)Addr[4]; //�������ѡ���MAC��ַ�ĸ�λ�Ĵ���ֵ
																												 //��ETH_MACA0HR��16λֵ
  /* Load the selected MAC address high register */
  (*(__IO UINT32 *) (ETH_MAC_ADDR_HBASE + MacAddr)) = tmpreg; //��������ĸ�λֵд��ETH_MAC0HR��
  /* Calculate the selected MAC address low register */
	//�������ѡ���MAC��ַ�ĵ�λ�Ĵ���ֵ,��ETH_MACA0LR��16λֵ
  tmpreg = ((UINT32)Addr[3] << 24) | ((UINT32)Addr[2] << 16) | ((UINT32)Addr[1] << 8) | Addr[0];

  /* Load the selected MAC address low register */
  (*(__IO UINT32 *) (ETH_MAC_ADDR_LBASE + MacAddr)) = tmpreg;//��������ĵ�λֵд��ETH_MAC0LR��
}

/*******************************************************************************
 * @fn          ETH_DMARxDescChainInit
 *
 * @brief       Receive desciptor iniliazation
 *
 * @param       DMARxDescTab:pointer to the receive desciptor table
 *              RxBuff:pointer to the receive buffer (receive queue)
 *              RxBuffCount:Number of receive desciptor or receive queue
 *
 * @return      None
 */
void ETH_DMARxDescChainInit(ETH_DMADESCTypeDef *DMARxDescTab, UINT8 *RxBuff, UINT32 RxBuffCount)
{
  UINT32 i ,j= 0;
  UINT32* p;
  ETH_DMADESCTypeDef *DMARxDesc;

  DMARxDescToGet = DMARxDescTab;

  for(i=0; i < RxBuffCount; i++)
  {
    DMARxDesc = DMARxDescTab+i;
    DMARxDesc->Status = ETH_DMARxDesc_OWN;
    DMARxDesc->ControlBufferSize =  (UINT32)ETH_RX_BUF_SIZE;
    DMARxDesc->Buffer1Addr = (UINT32)(&RxBuff[i*ETH_RX_BUF_SIZE]);
    if(i >= (RxBuffCount-1))
    {
      DMARxDesc->Buffer2NextDescAddr = (UINT32)(DMARxDescTab);
      DMARxDesc->Status |= ETH_DMARxDesc_RER;/* ��RER������� */
    }
    else
    {
      DMARxDesc->Buffer2NextDescAddr = (UINT32)(DMARxDescTab+i+1);
    }
#ifdef Enable_IEEE_1588
    DMARxDesc->Buffer1Addr_BK = DMARxDesc->Buffer1Addr;
    DMARxDesc->NextDescAddr_BK = DMARxDesc->Buffer2NextDescAddr;
#endif
    p=(void*)DMARxDesc;
#if 0
    printf("��%d������������:\n",i);
    for(j=0;j<(sizeof(ETH_DMADESCTypeDef))/4;j++)
    printf("addr:0x%08x:0x%08x.\n",(p+j),*(UINT32*)(p+j));
#endif
  }
  ETH->DMARDLAR = (UINT32) DMARxDescTab;
}

/*******************************************************************************
 * @fn        ETH_DMATxDescChainInit
 *
 * @brief     transmit desciptor iniliazation
 *
 * @param     DMARxDescTab:pointer to the transmit desciptor table
 *            RxBuff:pointer to the transmit buffer (transmit queue)
 *            RxBuffCount:Number of transmit desciptor or transmit queue
 *
 * @return    None
 */
void ETH_DMATxDescChainInit(ETH_DMADESCTypeDef *DMATxDescTab, UINT8 *TxBuff, UINT32 TxBuffCount)
{
  UINT32 i = 0, buf_addr;
  UINT8 *p;
  ETH_DMADESCTypeDef *DMATxDesc;

  DMATxDescToSet = DMATxDescTab;                               /* ��ǰ������ָ��ָ�������������׵�ַ */

  /* ע��CH565���������ĵĶ���֮�� */
  for(i=0; i < TxBuffCount; i++)
  {
    DMATxDesc = DMATxDescTab + i;
    DMATxDesc->Buffer1Addr = (UINT32)TxBuff+i*ETH_TX_BUF_SIZE;/* д��������ַ */                                         /* ��i�����Ͷ��еĵ�ַ */
//    DMATxDesc->Status |= ETH_DMATxDesc_OWN;                       /* ��ʼ��ʱ���ͷŷ�����������DMA */
    DMATxDesc->Status |= ETH_DMATxDesc_LS|ETH_DMATxDesc_FS|ETH_DMATxDesc_CIC_TCPUDPICMP_Full;         /* д״̬�֣��������ĵ�һ��32λ�� */
    DMATxDesc->ControlBufferSize|=ETH_TX_BUF_SIZE;  /* д���һ����������С������ */

    if(i>=TxBuffCount-1)
    {
    	DMATxDesc->Buffer2NextDescAddr = (UINT32) DMATxDescTab;   /* ָ���׵�ַ���ɻ� */
    	DMATxDesc->Status |= ETH_DMATxDesc_TER; /* ��TERλ */
    }
    else
    	DMATxDesc->Buffer2NextDescAddr = (UINT32)(DMATxDescTab+i+1);
#ifdef Enable_IEEE_1588
    DMATxDesc->Status |= ETH_DMATxDesc_TTSE;/* ʹ��ʱ��� */
    DMATxDesc->Buffer1Addr_BK = DMATxDesc->Buffer1Addr;/* д���û�������ַ */
    DMATxDesc->NextDescAddr_BK = DMATxDesc->Buffer2NextDescAddr;/* д������һ����������ַ */
#endif
  }
#if 0 /* �Ƿ�����ʼ�����ӡ��ȫ������������ */
  for(i=0;i<TxBuffCount;i++)
  {
#ifndef Enable_IEEE_1588
  printf("init Tx_des_addr:0x%08x:0x%08x 0x%08x 0x%08x 0x%08x\n",
#else
  printf("init Tx_des_addr:0x%08x:\n 0x%08x\n 0x%08x\n 0x%08x\n 0x%08x\n 0x%08x\n 0x%08x\n 0x%08x\n 0x%08x\n",
#endif
		   (UINT32) ((UINT8*)DMATxDescToSet+i*sizeof(ETH_DMADESCTypeDef)),\
		  *(UINT32*)((UINT8*)DMATxDescToSet+0x00+i*sizeof(ETH_DMADESCTypeDef)),\
		  *(UINT32*)((UINT8*)DMATxDescToSet+0x04+i*sizeof(ETH_DMADESCTypeDef)),\
		  *(UINT32*)((UINT8*)DMATxDescToSet+0x08+i*sizeof(ETH_DMADESCTypeDef)),\
		  *(UINT32*)((UINT8*)DMATxDescToSet+0x0c+i*sizeof(ETH_DMADESCTypeDef))
#ifdef Enable_IEEE_1588
		 ,*(UINT32*)((UINT8*)DMATxDescToSet+0x10+i*sizeof(ETH_DMADESCTypeDef)),\
		  *(UINT32*)((UINT8*)DMATxDescToSet+0x14+i*sizeof(ETH_DMADESCTypeDef)),\
		  *(UINT32*)((UINT8*)DMATxDescToSet+0x18+i*sizeof(ETH_DMADESCTypeDef)),\
		  *(UINT32*)((UINT8*)DMATxDescToSet+0x1c+i*sizeof(ETH_DMADESCTypeDef))
#endif
		  );
  }
#endif
//  printf("INIT:TDLAR: 0x%08x\n",DMATxDescTab);
  ETH->DMATDLAR = (UINT32)DMATxDescTab;                    /* ���÷����������б�Ļ���ַ */
//  printf("INIT:TDLAR: 0x%08X\n",ETH->DMATDLAR);
}

/*******************************************************************************
 * @fn       ETH_SoftwareReset
 *
 * @brief    ETH software reset
 *
 * @return   None
 */
void ETH_SoftwareReset(void)
{
  /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
  /* After reset all the registers holds their respective reset values */
  ETH->DMABMR |= ETH_DMABMR_SR;
}

/*******************************************************************************
 * @fn       ETH_GetSoftwareResetStatus
 *
 * @brief    Get software reset status
 *
 * @return   FlagStatus:software reset status
 */
FlagStatus ETH_GetSoftwareResetStatus(void)
{
  FlagStatus bitstatus = RESET;
  if((ETH->DMABMR & ETH_DMABMR_SR) != 0)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*******************************************************************************
 * @fn         ETH_DMAITConfig
 *
 * @brief      Configuration DMA interrupt
 *
 * @param      ETH_DMA_IT:Type of DMA interrupt
 *             NewState:Enabe DMA interrupt or Disable DMA interrupt
 *
 * @return     None
 */
void ETH_DMAITConfig(UINT32 ETH_DMA_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected ETHERNET DMA interrupts */
    ETH->DMAIER |= ETH_DMA_IT;
  }
  else
  {
    /* Disable the selected ETHERNET DMA interrupts */
    ETH->DMAIER &=(~(UINT32)ETH_DMA_IT);
  }
}

/*******************************************************************************
 * @fn        ETH_DMAClearITPendingBit
 *
 * @brief     Clear DMA interupt flag
 *
 * @param     ETH_DMA_IT:Type of DMA interrupt
 *
 * @return     None
 */
void ETH_DMAClearITPendingBit(UINT32 ETH_DMA_IT)
{
  /* Clear the selected ETHERNET DMA IT */
  ETH->DMASR = (UINT32) ETH_DMA_IT;
}

/*******************************************************************************
 * @fn        ETH_ReadPHYRegister
 *
 * @brief     Read PHY register
 *
 * @param     PHYAddress:PHY address
 *            PHYReg:PHY register address
 *
 * @return    Value of PHY register
 */
UINT16 ETH_ReadPHYRegister(UINT16 PHYAddress, UINT16 PHYReg)
{
  UINT32 tmpreg = 0;
__IO UINT32 timeout = 0;
  /* Check the parameters */

  /* Get the ETHERNET MACMIIAR value */
  tmpreg = ETH->MACMIIAR;
  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= ~MACMIIAR_CR_MASK;
  /* Prepare the MII address register value */
  tmpreg |=(((UINT32)PHYAddress<<11) & ETH_MACMIIAR_PA); /* Set the PHY device address */
  tmpreg |=(((UINT32)PHYReg<<6) & ETH_MACMIIAR_MR);      /* Set the PHY register address */
  tmpreg &= ~ETH_MACMIIAR_MW;                              /* Set the read mode */
  tmpreg |= ETH_MACMIIAR_MB;                               /* Set the MII Busy bit */
  /* Write the result value into the MII Address register */
  ETH->MACMIIAR = tmpreg;
  /* Check for the Busy flag */
  do
  {
    timeout++;
    tmpreg = ETH->MACMIIAR;
  } while ((tmpreg & ETH_MACMIIAR_MB) && (timeout < (UINT32)PHY_READ_TO));
  /* Return ERROR in case of timeout */
  if(timeout == PHY_READ_TO)
  {
    return (UINT16)ETH_ERROR;
  }

  /* Return data register value */
  return (UINT16)(ETH->MACMIIDR);
}

/*******************************************************************************
 * @fn          ETH_WritePHYRegister
 *
 * @brief       Write PHY register
 *
 * @param       PHYAddress:PHY address
 *              PHYReg:PHY register address
 *              PHYValue:Value will be written of PHY register
 *
 * @return      Execution status
 */
UINT32 ETH_WritePHYRegister(UINT16 PHYAddress, UINT16 PHYReg, UINT16 PHYValue)
{
  UINT32 tmpreg = 0;
  __IO UINT32 timeout = 0;

  /* Get the ETHERNET MACMIIAR value */
  tmpreg = ETH->MACMIIAR;
  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= ~MACMIIAR_CR_MASK;
  /* Prepare the MII register address value */
  tmpreg |=(((UINT32)PHYAddress<<11) & ETH_MACMIIAR_PA); /* Set the PHY device address */
  tmpreg |=(((UINT32)PHYReg<<6) & ETH_MACMIIAR_MR);      /* Set the PHY register address */
  tmpreg |= ETH_MACMIIAR_MW;                               /* Set the write mode */
  tmpreg |= ETH_MACMIIAR_MB;                               /* Set the MII Busy bit */
  /* Give the value to the MII data register */
  ETH->MACMIIDR = PHYValue;
  /* Write the result value into the MII Address register */
  ETH->MACMIIAR = tmpreg;
  /* Check for the Busy flag */
  do
  {
    timeout++;
    tmpreg = ETH->MACMIIAR;
  } while ((tmpreg & ETH_MACMIIAR_MB) && (timeout < (UINT32)PHY_WRITE_TO));
  /* Return ERROR in case of timeout */
  if(timeout == PHY_WRITE_TO)
  {
    return ETH_ERROR;
  }

  /* Return SUCCESS */
  return ETH_SUCCESS;
}

/*--------------------------file end-------------------------------*/
