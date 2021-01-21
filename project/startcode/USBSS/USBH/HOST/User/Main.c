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
UINT8V out_seq=0,in_seq=0;          //USB包序列号，每一个通道都需要一个

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
    USB30H_init(ENABLE);                  //USB3.0初始化

    while(1){
        if( device_link_status )
        {
            USB30_host_enum();   //USB枚举
            while(1){
//===========================OUT========================================================
                printf("OUT start\n");
                USBSSH->UH_TX_DMA = (UINT32)(UINT8 *)endpTXbuff;    //突发模式下需要重新指定缓冲区
                rcv_num = 4;
                sta = USB30H_OUT_Data(out_seq,rcv_num,2,1024);// 向端点2发送数据
                switch(sta&0x07){
                    case 1:     //ACK
                        break;
                    default:     //NRDY
                        printf("tNRDY\n");
                        i=0;
                        while( !USB30H_Erdy_Status(  &nump, &endp  ) ){//等待ERDY标志
                            i++;
                            if(i==2000000){  //ERDY超时
                                i=0;
                                sta = USB30H_OUT_Data(out_seq,rcv_num,2,1024);   //重试
                                if( (sta&0x07) == 1 )     break;
                            }
                        }
                        if((sta&0x07) == 2){
                            sta = USB30H_OUT_Data(out_seq,nump,2,1024);          //按照ERDY中长度重新发起事务
                        }
                        break;
                }
                out_seq += rcv_num;  //包序号增
                out_seq &= 0x1F;   //包序号循环判断
                printf("OUT done\n\n");

//===========================IN===========================================================
                printf("IN start\n");
                send_num = nump =  4;
                USBSSH->UH_RX_DMA = (UINT32)(UINT8 *)endpRXbuff;    //突发模式下需要重新指定缓冲区
                num = USB30H_IN_Data(in_seq, &send_num, 2 );// 从端点2获取数据
                switch( num & 0x7000 ){
                    case 0x1000:    //ACK
                        break;
                    default:        //NRDY
                        printf("rNRDY\n");
                        i=0;
                        while( !USB30H_Erdy_Status( &nump, &endp  ) ){             //等待ERDY标志
                            i++;
                            if(i==2000000){
                                i=0;
                                num = USB30H_IN_Data(in_seq, &send_num, 2 );    //重试
                                if( (num & 0x7000) == 0x1000 )    break;
                            }
                        }
                        if( (num & 0x7000) == 0x2000){
                            send_num = nump;                                 //按照ERDY中长度重新发起事务
                            num = USB30H_IN_Data(in_seq,&send_num, 2 );
                        }
                        break;
                }
                num &= 0x0fff;   //提取包数量
                in_seq += (nump-send_num);   //包序列增
                in_seq  &= 0x1F;    //包序列循环判断
                printf("IN done,last pack length:%d\n\n",num);

            }
        }
    }
}




