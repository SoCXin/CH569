/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"

#define  FREQ_SYS   80000000

#define  CMD_STATUS1         0x05
#define  CMD_WR_ENABLE       0x06
#define  CMD_ERASE_4KBYTE    0x20
#define  CMD_ERASE_32KBYTE   0x52
#define  CMD_READ_DATA       0x03
#define  CMD_PAGE_PROG       0x02
#define  CMD_FAST_READ       0x0B
#define  CMD_DEVICE_ID       0x90

/********************************* ���Ŷ��� ************************************
*    PA12  <===========>  SCS0
*    PA13  <===========>  SCK0
*    PA14  <===========>  MOSI0
*    PA15  <===========>  MISO0
*******************************************************************************/
#define  SPI0_CS_LOW()        R32_PA_CLR |= 1<<12
#define  SPI0_CS_HIGH()       R32_PA_OUT |= 1<<12

/*******************************************************************************
 * @fn        DebugInit
 *
 * @brief     Initializes the UART1 peripheral.
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

/*******************************************************************************
 * @fn      SPI_MASTER_INIT
 *
 * @brief   SPI0����ģʽ��ʼ��
 *
 * @return  None
 */
void SPI_MASTER_INIT(void)
{
  R8_SPI0_CTRL_MOD = RB_SPI_MOSI_OE|RB_SPI_SCK_OE;                              /* MOSI,SCK���ʹ�ܣ�����ģʽ����ʽ0 */
  R8_SPI0_CLOCK_DIV = 0x0a;                                                     /* 10��Ƶ��100/10=10M */
  R32_PA_DIR |= (1<<14 | 1<<13 | 1<<12);                                        /* MOSI(PA14),SCK0(PA13),SCS(PA12)Ϊ���*/
  R32_PA_PU  |=  1<<12 ;
  R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}

/*******************************************************************************
 * @fn   SPI0_Trans
 *
 * @brief ����һ�ֽ�����
 *
 * @param  data -Ҫ���͵�����
 *
 * @return  None
 */
void SPI0_Trans(UINT8 data)
{

//    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
//    R8_SPI0_BUFFER = data;
//    while( !(R8_SPI0_INT_FLAG & RB_SPI_FREE) );

    R32_SPI0_FIFO = data;
    R16_SPI0_TOTAL_CNT = 0x01;
    while( R8_SPI0_FIFO_COUNT != 0 );                                           /* �ȴ����ݷ������ */
}

/*******************************************************************************
 * @fn      SPI0_Recv
 *
 * @brief   ����һ�ֽ�����
 *
 * @return  None
 */
UINT8 SPI0_Recv(void)
{
//    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
//    R8_SPI0_BUFFER = 0xFF;                                //��������
//    while( !(R8_SPI0_INT_FLAG & RB_SPI_FREE) );
//    return ( R8_SPI0_BUFFER );

    UINT8 data;
    R32_SPI0_FIFO = 0xff;
    R16_SPI0_TOTAL_CNT = 0x01;
    while( R8_SPI0_FIFO_COUNT != 0 );                                           /* �ȴ����ݻ��� */
    data = R8_SPI0_BUFFER;
    return data;
}

/*******************************************************************************
 * @fn       SPI0_RecvS
 *
 * @brief    ʹ��FIFO�������ն��ֽ�
 *
 * @param    pbuf - �����͵����������׵�ַ
             len - �����͵����ݳ��ȣ����4095

 * @return   None
 */
void SPI0_RecvS(UINT8 *pbuf, UINT16 len)
{
    UINT16  readlen;

    readlen = len;
    R8_SPI0_CTRL_MOD |= RB_SPI_FIFO_DIR;          //�������ݷ���Ϊ����
    R16_SPI0_TOTAL_CNT = len;                     //������Ҫ���յ����ݳ��ȣ�FIFO����Ϊ���볤�Ȳ�Ϊ0�����������
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END;
    while( readlen )
    {
        if( R8_SPI0_FIFO_COUNT )
        {
            *pbuf = R8_SPI0_FIFO;
            pbuf++;
            readlen--;
        }
    }
}

/*******************************************************************************
 * @fn      ReadExternalFlashStatusReg_SPI
 *
 * @brief   ������ȡ״̬�Ĵ�����������״̬�Ĵ�����ֵ
 *
 * @return  ExFlashRegStatus
 */
UINT8 ReadExternalFlashStatusReg_SPI(void)
{
    UINT8 ExFlashRegStatus;


    SPI0_CS_LOW();
    SPI0_Trans( CMD_STATUS1 );                                          //���Ͷ�״̬�Ĵ���������
    ExFlashRegStatus = SPI0_Recv();                                     //��ȡ״̬�Ĵ���
    SPI0_CS_HIGH();

    return ExFlashRegStatus;
}

/*******************************************************************************
 * @fn      WaitExternalFlashIfBusy
 *
 * @brief   �ȴ�оƬ����(��ִ��Byte-Program, Sector-Erase, Block-Erase, Chip-Erase������)
 *
 * @return  None
 */
void WaitExternalFlashIfBusy(void)
{
    while ((ReadExternalFlashStatusReg_SPI())&0x01 == 0x01 )
    {
        ;    //�ȴ�ֱ��Flash����
    }
}

/*******************************************************************************
 * @fn       WriteExternalFlashEnable_SPI
 *
 * @brief    дʹ�ܣ�ͬ����������ʹ��д״̬�Ĵ���
 *
 * @return   None
 */
void WriteExternalFlashEnable_SPI(void)
{
    SPI0_CS_LOW();
    SPI0_Trans( CMD_WR_ENABLE );                                        //����дʹ������
    SPI0_CS_HIGH();
}

/*******************************************************************************
 * @fn       EraseExternal4KFlash_SPI
 *
 * @brief    ����4K Flash  ����һ������
 *
 * @param    Dst_Addr 0-1 ffff ffff ,��������ַ���ڵ�����
 *
 * @return   None
 */
void EraseExternal4KFlash_SPI(UINT32 Dst_Addr)
{
    WriteExternalFlashEnable_SPI();
    WaitExternalFlashIfBusy();

    SPI0_CS_LOW();
    SPI0_Trans(CMD_ERASE_4KBYTE);                                      //������������
    SPI0_Trans(((Dst_Addr & 0xFFFFFF) >> 16));                         //����3�ֽڵ�ַ
    SPI0_Trans(((Dst_Addr & 0xFFFF) >> 8));
    SPI0_Trans(Dst_Addr & 0xFF);
    SPI0_CS_HIGH();

    WaitExternalFlashIfBusy();
}

/*******************************************************************************
 * @fn         EraseExternalFlash_SPI
 *
 * @brief      ����32K Flash  ����һ������
 *
 * @param      Dst_Addr 0-1 ffff ffff ,��������ַ���ڵ�����
 *
 * @return     None
 */
void EraseExternal32KFlash_SPI(UINT32 Dst_Addr)
{
    WriteExternalFlashEnable_SPI();
    WaitExternalFlashIfBusy();

    SPI0_CS_LOW();
    SPI0_Trans(CMD_ERASE_32KBYTE);                                    //32K��������
    SPI0_Trans(((Dst_Addr & 0xFFFFFF) >> 16));                        //����3�ֽڵ�ַ
    SPI0_Trans(((Dst_Addr & 0xFFFF) >> 8));
    SPI0_Trans(Dst_Addr & 0xFF);
    SPI0_CS_HIGH();

    WaitExternalFlashIfBusy();
}

/*******************************************************************************
 * @fn           PageWriteExternalFlash_SPI
 *
 * @brief        ҳд��SPI��һҳ��д������256���ֽڵ�����
 *
 * @param        RcvBuffer - ���ݴ洢��
 *               StarAddr - ��ʼд��ĵ�ַ
 *               Len - Ҫд����ֽ���(���256),������Ӧ�ó�����ҳʣ����ֽ���
 *
 * @returnNone
 */
void PageWriteExternalFlash_SPI(UINT32 StarAddr, UINT16 Len, PUINT8 RcvBuffer)
{
    UINT16 i;

    WriteExternalFlashEnable_SPI();                                   //SET WEL

    SPI0_CS_LOW();
    SPI0_Trans(CMD_PAGE_PROG);                                        //����дҳ����
    SPI0_Trans(((StarAddr & 0xFFFFFF) >> 16));                        //����24bit��ַ
    SPI0_Trans(((StarAddr & 0xFFFF) >> 8));
    SPI0_Trans(StarAddr & 0xFF);
    for(i=0; i!=Len; i++){
    	SPI0_Trans(RcvBuffer[i]);    //ѭ��д��
    }
    SPI0_CS_HIGH();

    WaitExternalFlashIfBusy();                                        //�ȴ�д�����
}

/*******************************************************************************
 * @fn       BlukWriteExternalFlash_SPI
 *
 * @brief    �޼���дSPI FLASH
 *               ����ȷ����д��ַ��Χ�ڵ�����ȫ��Ϊ0XFF�������ڷ�0XFF��д������ݽ�ʧ�ܣ�
 *
 * @param    SendBuffer - ���ݴ洢��
 *           StarAddr - ��ʼд��ĵ�ַ
 *           Len - Ҫд����ֽ���(���65535)
 *
 * @return   None
 */
void BlukWriteExternalFlash_SPI(UINT32 StarAddr, UINT16 Len, PUINT8 SendBuffer)
{
    UINT16  pageremain;

    pageremain = 256-StarAddr%256;                                     //��ҳʣ����ֽ���
    if(Len<=pageremain)
    {
        pageremain=Len;                                                //������256���ֽ�
    }
    while(1)
    {
        PageWriteExternalFlash_SPI(StarAddr,pageremain,SendBuffer);
        if(Len==pageremain)
        {
            break;                                                     //д�������
        }
        else
        {
            SendBuffer+=pageremain;
            StarAddr+=pageremain;
            Len-=pageremain;                                           //��ȥ�Ѿ�д����ֽ���
            if(Len>256)
            {
                pageremain=256;                                        //һ�ο���д��256���ֽ�
            }
            else
            {
                pageremain=Len;                                        //����256���ֽ�
            }
        }
    }
}

/*******************************************************************************
 * @fn  ReadExternalFlash_SPI
 *
 * @brief ��ȡ��ַ������
 *
 * @param     StarAddr
 *            Len ��ȡ���ݳ���
 *            RcvBuffer ���ջ�������ʼ��ַ
 *
 * @return None
 */
void ReadExternalFlash_SPI(UINT32 StarAddr, UINT16 Len, PUINT8 RcvBuffer)
{
    SPI0_CS_LOW();
    SPI0_Trans(CMD_READ_DATA);                                         //������
    SPI0_Trans(((StarAddr & 0xFFFFFF) >> 16));                         //����3�ֽڵ�ַ
    SPI0_Trans(((StarAddr & 0xFFFF) >> 8));
    SPI0_Trans(StarAddr & 0xFF);
    SPI0_RecvS( RcvBuffer, Len );
    SPI0_CS_HIGH();
}

/*******************************************************************************
 * @fn        BlukReadExternalFlash_SPI
 *
 * @brief     ��ȡ��ʼ��ַ�ڶ���ֽڵ����ݣ����뻺������
 *
 * @param     StarAddr -Destination Address 000000H - 1FFFFFH
              Len - ��ȡ���ݳ���
              RcvBuffer - ���ջ�������ʼ��ַ

 * @return None
 */
void BlukReadExternalFlash_SPI(UINT32 StarAddr, UINT16 Len, PUINT8 RcvBuffer)
{
    SPI0_CS_LOW();
    SPI0_Trans(CMD_FAST_READ);                                         //���ٶ�
    SPI0_Trans(((StarAddr & 0xFFFFFF) >> 16));                         //����3�ֽڵ�ַ
    SPI0_Trans(((StarAddr & 0xFFFF) >> 8));
    SPI0_Trans(StarAddr & 0xFF);
    SPI0_Trans(0x00);
    SPI0_RecvS( RcvBuffer, Len );
    SPI0_CS_HIGH();
}

/*******************************************************************************
 * @fn        SPIFlash_ReadID
 *
 * @brief     SPI Flash��ȡоƬID
 *
 * @return    0XEF13 - ��ʾоƬ�ͺ�ΪW25Q80
 *            0XEF14 - ��ʾоƬ�ͺ�ΪW25Q16
 *            0XEF15 - ��ʾоƬ�ͺ�ΪW25Q32
 *            0XEF16 - ��ʾоƬ�ͺ�ΪW25Q64
 *            0XEF17 - ��ʾоƬ�ͺ�ΪW25Q128
 */
UINT16 SPIFlash_ReadID(void)
{
    UINT16  temp = 0;

    R32_PA_CLR |=  1<<12 ;

    SPI0_Trans(0x90);                    //��ȡID����
    SPI0_Trans(0x00);
    SPI0_Trans(0x00);
    SPI0_Trans(0x00);
    temp = SPI0_Recv();
    temp = temp<<8;
    temp |= SPI0_Recv();

    R32_PA_OUT |=  1<<12 ;

    return temp;
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main()
{

	UINT8 buf[1024];
	UINT8 i;

	SystemInit(FREQ_SYS);
    Delay_Init(FREQ_SYS);


    /*���ô��ڵ��� */
	DebugInit(115200);
	printf("Start @ChipID=%02X\r\n", R8_CHIP_ID );

	SPI_MASTER_INIT ( );                                                       /* SPI0����ģʽ��ʼ�� */

    printf("START SPI FLASH\n");

    printf("id:0x%04x\n", SPIFlash_ReadID() );                                  /*��ȡоƬID */

    for(i=0; i!=255; i++){
        buf[i] = i;
    }


    EraseExternal4KFlash_SPI(0);
    BlukWriteExternalFlash_SPI(0,255,buf);
    BlukReadExternalFlash_SPI( 0,255,buf );

    for(i=0; i!=255; i++){
    	printf("%d ",(UINT16)buf[i]);
    }
    printf("done\n");


    while(1);    
}



