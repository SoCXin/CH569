/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/12/23
* Description 		 : Main program body
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"
#include "CH56xusb30h_LIB.h"
#include "CH56x_usb30h.h"
#include "CH56x_host_hs.h"

/* Global define */
#define  FREQ_SYS      120000000

#define BULK_IN        0
#define BULK_OUT       1
#define DUG_FUNC_EN    0

/* Global Variable */
UINT8V U30_Check_Time = 0;
volatile DevInfo g_DevInfo;
extern UINT8V g_DeviceConnectstatus;
extern UINT8V g_DeviceUsbType;

/* Function  */
void TMR0_IRQHandler( void ) __attribute__((interrupt("WCH-Interrupt-fast")));
void U30_BulkTest(void);

/*******************************************************************************
 * @fn        TMR0_IRQHandler
 *
 * @briefI    TMR0 handler.
 *
 * @return    None
 */
void TMR0_IRQHandler( void )
{
   if( R8_TMR0_INT_FLAG & RB_TMR_IF_CYC_END )
   {
      R8_TMR0_INT_FLAG = RB_TMR_IF_CYC_END;
      if( g_DeviceConnectstatus == USB_INT_CONNECT_U20 )
      {
        U30_Check_Time++;
        if( U30_Check_Time>= 10 ) //1S�ĳ�ʱʱ��
        {
          g_DeviceConnectstatus = USB_INT_CONNECT;
          U30_Check_Time = 0;
          TMR0_ITCfg( DISABLE ,RB_TMR_IE_CYC_END);
          PFIC_EnableIRQ(TMR0_IRQn);
        }
      }
   }
}

/*******************************************************************************
 * @fn        DebugInit
 *
 * @briefI    nitializes the UART1 peripheral.
 *
 * @param     baudrate - UART1 communication baud rate.
 *
 * @return    None
 */
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

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main( )
{
    UINT8 ret;
    SystemInit(FREQ_SYS);
    Delay_Init(FREQ_SYS);
    mDelaymS(10);                        //delay
    DebugInit(921600);
    PRINT("\n\nThis is USB3.0 host program(120MHz)\n");

    PFIC_EnableIRQ(LINK_IRQn);            //enable USBSSH LINK global interrupt
    USBHS_Host_Init(ENABLE);              //USB2.0��ʼ��

    TMR0_TimerInit(8000000);              //3.0link��ʱ��ʱ����ʼ��
    TMR0_ITCfg( ENABLE ,RB_TMR_IE_CYC_END);
    PFIC_EnableIRQ( TMR0_IRQn );

    while(1)
    {
        if( g_DeviceConnectstatus == USB_INT_CONNECT )                 //��USB�豸����
        {
            if( g_DeviceUsbType == USB_U30_SPEED )                     //3.0�豸����
            {
                USB30_Host_Enum();
#if DUG_FUNC_EN
                U30_BulkTest();
#endif
            }
            else                                                       //2.0�豸����
            {
                mDelaymS(5);
                ret = USBHS_Host_Enum( endpRXbuff );
                if(ret != ERR_SUCCESS)
                {
                    printf("enumerate failed\n");
                }
                else {
                }
            }
            printf("wait_disconnect\n");
            while(g_DeviceConnectstatus == USB_INT_CONNECT)            //�ȴ��豸�Ͽ�
            {
              mDelaymS( 100 );
              if( g_DeviceUsbType == USB_U20_SPEED )
              {
                R8_USB_INT_FG = RB_USB_IF_DETECT;
                if(!(R8_USB_MIS_ST & RB_USB_ATTACH) )   break;         //��ǰ��usb�豸
              }
            }
            g_DeviceConnectstatus = 0;
            g_DeviceUsbType = 0;
            USB30H_init(DISABLE);
            USBHS_Host_Init(DISABLE);
            USBHS_Host_Init(ENABLE);                                 //�豸�γ��� ����2.0��ʼ��
            printf("disconnect\n");
        }
        else if(R8_USB_INT_FG & RB_USB_IF_DETECT)                   //�Ƚ���USB2.0�豸���ӣ�֮���ڽ���USB3.0�豸���ӳ�ʼ��
        {
            R8_USB_INT_FG = RB_USB_IF_DETECT;
            if( R8_USB_MIS_ST & RB_USB_ATTACH )
            {
                printf("USB2.0 DEVICE ATTACH !\n");
                g_DeviceUsbType = USB_U20_SPEED;
                g_DeviceConnectstatus = USB_INT_CONNECT_U20;
                mDelaymS(100);
                TMR0_ITCfg( ENABLE ,RB_TMR_IE_CYC_END);
                PFIC_EnableIRQ(TMR0_IRQn);
                USB30H_init(ENABLE);                      //��ʼ��USB3.0�Ĵ���
                SetBusReset();                            //����λ
            }
            else
            {
                g_DeviceConnectstatus = USB_INT_DISCONNECT;
                printf("USB2.0 DEVICE DEATTACH !\n");
            }
        }
    }
}

/*******************************************************************************
 * @fn        para_init
 *
 * @briefI    parameter initialize
 *
 * @param     pusbdev
 *
 * @return    None
 */
void para_init(void)
{
    UINT8 i;
    for(i=0;i<8;i++)
    {
        g_DevInfo.InSeqNum[i]= 0;
        g_DevInfo.OutSeqNum[i]= 0  ;          //0-7�Ŷ˵��������0
        g_DevInfo.InMaxBurstSize[i]= 0;
        g_DevInfo.OutMaxBurstSize[i]= 0  ;    //�˵����ͻ����������ʼ��
    }
}

/*********************************************************************
 * @fn      U30_BulkTest
 *
 * @brief   Bulk test.
 *
 * @return  none
 */
void U30_BulkTest(void)
{
    UINT8   nump,send_num,rcv_num;
    UINT16  len;
    UINT32 pack_num = 0;

    para_init();
    while(1)
    {
#if BULK_IN
//===========================IN===========================================================
       USBSSH->UH_RX_DMA = (UINT32)(UINT8 *)endpRXbuff;                                  //ͻ��ģʽ����Ҫ����ָ��������
       rcv_num  = g_DevInfo.InMaxBurstSize[ENDP_2] ;                                     //����Ҫȡ�İ�����
       len = USBSS_INTransaction(g_DevInfo.InSeqNum[ENDP_2], &rcv_num, ENDP_2 );         //�Ӷ˵�2��ȡ����

       g_DevInfo.InSeqNum[ENDP_2] += (g_DevInfo.InMaxBurstSize[ENDP_2]- rcv_num);        //����������
       g_DevInfo.InSeqNum[ENDP_2] &= 0x1F;                                               //������0~31ѭ���ж�

#else
//===========================OUT========================================================
       USBSSH->UH_TX_DMA = (UINT32)(UINT8 *)endpTXbuff;                                  //ͻ��ģʽ����Ҫ����ָ��������
       send_num = g_DevInfo.OutMaxBurstSize[ENDP_2] ;                                    //����Ҫ���͵İ�����
       nump = USBSS_OUTTransaction( g_DevInfo.OutSeqNum[ENDP_2] ,send_num ,ENDP_2 ,1024);

       g_DevInfo.OutSeqNum[ENDP_2] += send_num - nump;                                   //���������
       g_DevInfo.OutSeqNum[ENDP_2] &= 0x1F;                                              //�����0~31ѭ���ж�

#endif
    }

}


