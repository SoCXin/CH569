/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
*******************************************************************************/

#define  FREQ_SYS   80000000
#include "CH56x_common.h"

UINT8 TxBuff[]="This is a Tx exam\r\n";
UINT8 RxBuff[100];
UINT8 trigB;

void UART2_IRQHandler (void) __attribute__((interrupt()));
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
int main()
{
	SystemInit(FREQ_SYS);
	Delay_Init(FREQ_SYS);

/* ���Դ��ڳ�ʼ�� */
	DebugInit(115200);
	PRINT("Start @ChipID=%02X\r\n", R8_CHIP_ID );

/* ���ݳ��ȱ��� */
	UINT8 len;

/* ���ô��� */
	GPIOA_SetBits(GPIO_Pin_3);
	GPIOA_ModeCfg(GPIO_Pin_2, GPIO_ModeIN_PU_NSMT);			// RXD-��������
	GPIOA_ModeCfg(GPIO_Pin_3, GPIO_Slowascent_PP_8mA);		// TXD-�������
	UART2_DefInit();

#if 0       // ���ڷ����ַ���
	UART2_SendString( TxBuff, sizeof(TxBuff) );

#endif

#if 0       // ��ѯ �����귢��
	while(1)
	{
		len = UART2_RecvString(RxBuff);
		if( len )
		{
			UART2_SendString( RxBuff, len );
		}
	}

#endif

#if 1      // �жϷ�ʽ
	UART2_ByteTrigCfg( UART_7BYTE_TRIG );
	trigB = 7;
	UART2_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	PFIC_EnableIRQ( UART2_IRQn );
#endif

	while(1)
	{
		PRINT("11111111\r\n");
		mDelaymS(1000);
	}
}



/*******************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : Interruption function
* Input          : None
* Return         : None
*******************************************************************************/
void UART2_IRQHandler(void)
{
	UINT8 i;
	switch( UART2_GetITFlag() )
	{
		case UART_II_LINE_STAT:        //��·״̬����
			PRINT("UART2_GetLinSTA()\r\n",UART2_GetLinSTA());
			break;

		case UART_II_RECV_RDY:          //���ݴﵽ������
			for(i=0; i!=trigB; i++)
			{
				RxBuff[i] = UART2_RecvByte();
				UART2_SendByte(RxBuff[i]);
			}

			break;

		case UART_II_RECV_TOUT:         //���ճ�ʱ
			i = UART2_RecvString(RxBuff);
			UART2_SendString( RxBuff, i );
			break;

		case UART_II_THR_EMPTY:         //���ͻ�������
			break;

		case UART_II_MODEM_CHG:         //�����ڴ���0
			break;

		default:
			break;
	}
}
