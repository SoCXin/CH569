/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/12/23
* Description 		 : Main program body
*******************************************************************************/

#include "CH56x_common.h"
#include "CH56xusb30h_LIB.h"
#include "CH56x_usb30h.h"


/* Global define */


/* Global Variable */
UINT8V out_seq=0,in_seq=0;          //USB�����кţ�ÿһ��ͨ������Ҫһ��

/* Function declaration */



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


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main( )
{
    UINT16  num=0;
    UINT16 sta=0;
    UINT8 nump,endp,send_num,rcv_num;
    UINT32  i;
    SystemInit(FREQ_SYS);
    Delay_Init(FREQ_SYS);
    mDelaymS(10);                        //delay
    DebugInit(921600);
    PRINT("\n\nThis is USB3.0 host program(120MHz)\n");

    PFIC_EnableIRQ(LINK_IRQn);            //enable USBSSH global interrupt
    PFIC_EnableIRQ(USBSS_IRQn);
    USB30H_init(ENABLE);                  //USB3.0��ʼ��

    while(1){
        if( device_link_status )
        {
            USB30_host_enum();   //USBö��
            while(1){
//===========================OUT========================================================
                printf("OUT start\n");
                USBSSH->UH_TX_DMA = (UINT32)(UINT8 *)endpTXbuff;    //ͻ��ģʽ����Ҫ����ָ��������
                rcv_num = 4;
                sta = USB30H_OUT_Data(out_seq,rcv_num,2,1024);// ��˵�2��������
                switch(sta&0x07){
                    case 1:     //ACK
                        break;
                    default:     //NRDY
                        printf("tNRDY\n");
                        i=0;
                        while( !USB30H_Erdy_Status(  &nump, &endp  ) ){//�ȴ�ERDY��־
                            i++;
                            if(i==2000000){  //ERDY��ʱ
                                i=0;
                                sta = USB30H_OUT_Data(out_seq,rcv_num,2,1024);   //����
                                if( (sta&0x07) == 1 )     break;
                            }
                        }
                        if((sta&0x07) == 2){
                            sta = USB30H_OUT_Data(out_seq,nump,2,1024);          //����ERDY�г������·�������
                        }
                        break;
                }
                out_seq += rcv_num;  //�������
                out_seq &= 0x1F;   //�����ѭ���ж�
                printf("OUT done\n\n");

//===========================IN===========================================================
                printf("IN start\n");
                send_num = nump =  4;
                USBSSH->UH_RX_DMA = (UINT32)(UINT8 *)endpRXbuff;    //ͻ��ģʽ����Ҫ����ָ��������
                num = USB30H_IN_Data(in_seq, &send_num, 2 );// �Ӷ˵�2��ȡ����
                switch( num & 0x7000 ){
                    case 0x1000:    //ACK
                        break;
                    default:        //NRDY
                        printf("rNRDY\n");
                        i=0;
                        while( !USB30H_Erdy_Status( &nump, &endp  ) ){             //�ȴ�ERDY��־
                            i++;
                            if(i==2000000){
                                i=0;
                                num = USB30H_IN_Data(in_seq, &send_num, 2 );    //����
                                if( (num & 0x7000) == 0x1000 )    break;
                            }
                        }
                        if( (num & 0x7000) == 0x2000){
                            send_num = nump;                                 //����ERDY�г������·�������
                            num = USB30H_IN_Data(in_seq,&send_num, 2 );
                        }
                        break;
                }
                num &= 0x0fff;   //��ȡ������
                in_seq += (nump-send_num);   //��������
                in_seq  &= 0x1F;    //������ѭ���ж�
                printf("IN done,last pack length:%d\n\n",num);

            }
        }
    }
}




