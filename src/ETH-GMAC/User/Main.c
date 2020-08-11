/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : CH565/569 ǧ����̫����·���շ��ӿں�����ʱ����
*******************************************************************************/

#include "CH56x_common.h"
#include "ethernet_driver.h"
#include "ethernet_config.h"

/*  */
#define  FREQ_SYS   80000000 /* ������̫��ʱ����Ƶ���õ���60MHz */

/* MAC�շ������ڴ����� */
__attribute__ ((aligned(16)))
__attribute__((section(".dmadata")))
UINT8 MAC_QUEUE[TxDscrOffset*TX_Des_Num+                     /* ������������Ҫ�Ŀռ� */
				RxDscrOffset*RX_Des_Num+                     /* ������������Ҫ�Ŀռ� */
				TX_Des_Num*TX_Buf_Size+                      /* ���ͻ�������Ҫ�Ŀռ� */
				RX_Des_Num*RX_Buf_Size];                     /* ���ջ�������Ҫ�Ŀռ� */

ETH_DMADESCTypeDef  *DMATxDescToSet;                        /* ��ǰ����������ָ�� */
ETH_DMADESCTypeDef  *DMARxDescToGet;                        /* ��ǰ����������ָ�� */

extern Globe_RxDes_status_t Globe_RxDes_status;
UINT8 local_mac[6]={0x84,0xc2,0xe4,0x01,0x02,0x03,};


/*******************************************************************************
* Function Name  : DebugInit
* Description    : Initializes the UART1 peripheral.
* Input          : baudrate: UART1 communication baud rate.
* Return         : None
*******************************************************************************/
void DebugInit(UINT32 baudrate)		
{
	UINT32 x;
	UINT32 t = FREQ_SYS;
	
	x = 10 * t * 2 / 16 / baudrate;
	x = ( x + 5 ) / 10;
	R8_UART1_DIV = 1;
	R16_UART1_DL = x;
	R8_UART1_FCR = RB_FCR_FIFO_TRIG | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN;
	R8_UART1_LCR = RB_LCR_WORD_SZ;
	R8_UART1_IER = RB_IER_TXD_EN;
	R32_PA_SMT |= (1<<8) |(1<<7);
	R32_PA_DIR |= (1<<8);
}

#if 1
UINT8 ARP_package[]=
{
	0x40, 0xb0, 0x34, 0x98, 0x6b, 0x58,/* Ŀ��MAC */
	0x84, 0xc2, 0xe4, 0x01, 0x02, 0x03,/* ԴMAC */
	0x08,0x06,/* ARP�� */
	0x00,0x01,/* Ӳ������ */
	0x08,0x00,/* Э������ */
	0x06,0x04,/* Ӳ����ַ���Ⱥ�Э���ַ���� */
	0x00,0x01,/* ����� */
	0x84, 0xc2, 0xe4, 0x01, 0x02, 0x03,/* ԴMAC��ַ */
	0xc0,0xa8,0x1,0x0f,/* ԴЭ���ַ */
	0x00,0x00,0x00,0x00,0x00,0x00,/* Ŀ��MAC��ַ */
	0xc0,0xa8,0x1,0x0a,/* ԴЭ���ַ */
#if 1 /*padding 16bytes*5*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#endif
};
#endif

#define  target_msg_ptr ARP_package

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main()
{  
	UINT8 rc,i,enable_send;/* ״̬ */
	UINT8 *read_ptr,**p=&read_ptr;
	UINT16 read_length;

    SystemInit(FREQ_SYS);
    Delay_Init(FREQ_SYS);

    /* ���ô��ڵ��� */
	DebugInit(115200);
	PRINT("Start @ChipID=%02X\r\n", R8_CHIP_ID );
	PRINT("WCH Gigabits Ethernet MAC driver program demo! \n");
	PRINT("System frequence is :%d Hz,Complair at  %s,%s\n",FREQ_SYS,__DATE__,__TIME__);
	rc=Mac_init();
	while(rc)
	{
		PRINT("MAC init failed.retrying...\n");
		mDelaymS(2);
		rc=Mac_init();
	}
	enable_send=1;
	RGMII_TXC_Delay(0,5);/* RGMII�����ӳٺ���λ���� */
	Check_TxDes();

    while(1)
    {
		if(Globe_RxDes_status.pengding_RxDes_cnt)
		{
			read_length=mac_rece(p);
			enable_send=1;
			/* ������read_ptrָ��������У�����Ϊread_length */
			PRINT("rece:");
			for(i=0;i<6;i++)
				PRINT("%02x ",read_ptr[i]);
			for(i=6;i<12;i++)
				PRINT("%02x ",read_ptr[i]);
			for(i=12;i<22;i++)
				PRINT("%02x ",read_ptr[i]);
			PRINT("END!\n\n");
		}
		if(enable_send)
		{
			memcpy(get_txbuff_addr(),target_msg_ptr,sizeof(target_msg_ptr));
			mac_send(sizeof(target_msg_ptr));
			enable_send=0;
			PRINT("����������֡:%d\n",ETH->MMCTGFCR);
		}
    }
}





