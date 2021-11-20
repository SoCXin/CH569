/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
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
* Function Name  : SPI_MASTER_INIT
* Description    : SPI0����ģʽ��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_MASTER_INIT ( void )
{
  R8_SPI0_CTRL_MOD = RB_SPI_MOSI_OE|RB_SPI_SCK_OE;                              /* MOSI,SCK���ʹ�ܣ�����ģʽ����ʽ0 */
  R8_SPI0_CLOCK_DIV = 0x0a;                                                     /* 10��Ƶ��100/10=10M */
  R32_PA_DIR |= (1<<14 | 1<<13 | 1<<12);                                        /* MOSI(PA14),SCK0(PA13),SCS(PA12)Ϊ���*/
  R32_PA_PU  |=  1<<12 ;
  R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}

/*******************************************************************************
* Function Name  : SPI0_Trans
* Description    : ����һ�ֽ�����
* Input          : data -Ҫ���͵�����
* Output         : None
* Return         : None
*******************************************************************************/
void SPI0_Trans( UINT8 data )
{

//    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
//    R8_SPI0_BUFFER = data;
//    while( !(R8_SPI0_INT_FLAG & RB_SPI_FREE) );

    R32_SPI0_FIFO = data;
    R16_SPI0_TOTAL_CNT = 0x01;
    while( R8_SPI0_FIFO_COUNT != 0 );                                           /* �ȴ����ݷ������ */
}

/*******************************************************************************
* Function Name  : SPI0_Recv
* Description    : ����һ�ֽ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 SPI0_Recv( void )
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
* Function Name  : SPI0_RecvS
* Description    : ʹ��FIFO�������ն��ֽ�
* Input          : pbuf: �����͵����������׵�ַ
                   len: �����͵����ݳ��ȣ����4095
* Return         : None
*******************************************************************************/
void SPI0_RecvS( UINT8 *pbuf, UINT16 len )
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
* Function Name  : ReadExternalFlashStatusReg_SPI
* Description    : ������ȡ״̬�Ĵ�����������״̬�Ĵ�����ֵ
* Input          : None
* Output         : None
* Return         : ExFlashRegStatus
*******************************************************************************/
UINT8 ReadExternalFlashStatusReg_SPI( void )
{
    UINT8 ExFlashRegStatus;


    SPI0_CS_LOW();
    SPI0_Trans( CMD_STATUS1 );                                          //���Ͷ�״̬�Ĵ���������
    ExFlashRegStatus = SPI0_Recv();                                     //��ȡ״̬�Ĵ���
    SPI0_CS_HIGH();

    return ExFlashRegStatus;
}

/*******************************************************************************
* Function Name  : WaitExternalFlashIfBusy
* Description    : �ȴ�оƬ����(��ִ��Byte-Program, Sector-Erase, Block-Erase, Chip-Erase������)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WaitExternalFlashIfBusy( void )
{
    while ((ReadExternalFlashStatusReg_SPI())&0x01 == 0x01 )
    {
        ;    //�ȴ�ֱ��Flash����
    }
}

/*******************************************************************************
* Function Name  : WriteExternalFlashEnable_SPI
* Description    : дʹ�ܣ�ͬ����������ʹ��д״̬�Ĵ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WriteExternalFlashEnable_SPI( void )
{
    SPI0_CS_LOW();
    SPI0_Trans( CMD_WR_ENABLE );                                        //����дʹ������
    SPI0_CS_HIGH();
}

/*******************************************************************************
* Function Name  : EraseExternal4KFlash_SPI
* Description    : ����4K Flash  ����һ������
* Input          : Dst_Addr 0-1 ffff ffff ,��������ַ���ڵ�����
* Output         : None
* Return         : None
*******************************************************************************/
void EraseExternal4KFlash_SPI( UINT32 Dst_Addr )
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
* Function Name  : EraseExternalFlash_SPI
* Description    : ����32K Flash  ����һ������
* Input          : Dst_Addr 0-1 ffff ffff ,��������ַ���ڵ�����
* Output         : None
* Return         : None
*******************************************************************************/
void EraseExternal32KFlash_SPI( UINT32 Dst_Addr )
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
* Function Name  : PageWriteExternalFlash_SPI
* Description    : ҳд��SPI��һҳ��д������256���ֽڵ�����
* Input          : RcvBuffer:���ݴ洢��
*                  StarAddr:��ʼд��ĵ�ַ
*                  Len:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳʣ����ֽ���!!!
* Output         : None
* Return         : None
*******************************************************************************/
void PageWriteExternalFlash_SPI(UINT32 StarAddr,UINT16 Len,PUINT8 RcvBuffer)
{
    UINT16 i;

    WriteExternalFlashEnable_SPI();                                   //SET WEL

    SPI0_CS_LOW();
    SPI0_Trans(CMD_PAGE_PROG);                                        //����дҳ����
    SPI0_Trans(((StarAddr & 0xFFFFFF) >> 16));                        //����24bit��ַ
    SPI0_Trans(((StarAddr & 0xFFFF) >> 8));
    SPI0_Trans(StarAddr & 0xFF);
    for(i=0; i!=Len; i++)
    {
    	SPI0_Trans(RcvBuffer[i]);    //ѭ��д��
    }
    SPI0_CS_HIGH();

    WaitExternalFlashIfBusy();                                        //�ȴ�д�����
}

/*******************************************************************************
* Function Name  : BlukWriteExternalFlash_SPI
* Description    : �޼���дSPI FLASH
*                  ����ȷ����д��ַ��Χ�ڵ�����ȫ��Ϊ0XFF�������ڷ�0XFF��д������ݽ�ʧ�ܣ�
* Input          : SendBuffer:���ݴ洢��
*                  StarAddr:��ʼд��ĵ�ַ
*                  Len:Ҫд����ֽ���(���65535)
* Output         : None
* Return         : None
*******************************************************************************/
void BlukWriteExternalFlash_SPI(UINT32 StarAddr,UINT16 Len,PUINT8 SendBuffer)
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
* Function Name  : ReadExternalFlash_SPI
* Description    : ��ȡ��ַ������
*******************************************************************************/
void ReadExternalFlash_SPI( UINT32 StarAddr, UINT16 Len, PUINT8 RcvBuffer )
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
* Function Name  : BlukReadExternalFlash_SPI
* Description    : ��ȡ��ʼ��ַ�ڶ���ֽڵ����ݣ����뻺������
* Input          : StarAddr -Destination Address 000000H - 1FFFFFH
                   Len ��ȡ���ݳ���
                   RcvBuffer ���ջ�������ʼ��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void BlukReadExternalFlash_SPI( UINT32 StarAddr, UINT16 Len, PUINT8 RcvBuffer )
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
* Function Name  : SPIFlash_ReadID
* Description    : SPI Flash��ȡоƬID
* Input          : None
* Output         : None
* Return         : 0XEF13,��ʾоƬ�ͺ�ΪW25Q80
*                  0XEF14,��ʾоƬ�ͺ�ΪW25Q16
*                  0XEF15,��ʾоƬ�ͺ�ΪW25Q32
*                  0XEF16,��ʾоƬ�ͺ�ΪW25Q64
*                  0XEF17,��ʾоƬ�ͺ�ΪW25Q128
*******************************************************************************/
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

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
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

    for(i=0; i!=255; i++)
    {
        buf[i] = i;
    }


    EraseExternal4KFlash_SPI(0);
    BlukWriteExternalFlash_SPI(0,255,buf);
    BlukReadExternalFlash_SPI( 0,255,buf );

    for(i=0; i!=255; i++)
    {
    	printf("%d ",(UINT16)buf[i]);
    }
    printf("done\n");


    while(1);    
}



