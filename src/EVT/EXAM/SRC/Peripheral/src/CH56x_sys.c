/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_sys.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description        : This file contains all the functions prototypes for 
*                      SystemCoreClock, UART Printf , Delay functions .
*******************************************************************************/

#include "CH56x_common.h"


static uint8_t  p_us=0;
static uint16_t p_ms=0;

/*******************************************************************************
* Function Name  : Delay_Init
* Description    : Initializes Delay Funcation.
* Input          : systemclck:ϵͳʱ�� Hz
* Return         : None
*******************************************************************************/
void Delay_Init(uint32_t systemclck)
{
	p_us=systemclck/8000000;
	p_ms=(uint16_t)p_us*1000;
}

/*******************************************************************************
* Function Name  : mDelayuS
* Description    : Microsecond Delay Time.
* Input          : n��Microsecond number.
* Return         : None
*******************************************************************************/
void mDelayuS(uint32_t n)
{
	uint32_t i;

	SysTick->CTLR = 0;
	i = (uint32_t)n*p_us;

	SysTick->CMP = i;
	SysTick->CTLR = (1<<8)|(1<<0);

    while((SysTick->CNTFG & (1<<1)) != (1<<1));
    SysTick->CNTFG &= ~(1<<1);
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    : Millisecond Delay Time.
* Input          : n��Millisecond number.
* Return         : None
*******************************************************************************/
void mDelaymS(uint32_t n)
{
	uint32_t i;

	SysTick->CTLR = 0;
	i = (uint32_t)n*p_ms;

	SysTick->CMP = i;
	SysTick->CTLR = (1<<8)|(1<<0);

    while((SysTick->CNTFG & (1<<1)) != (1<<1));
    SysTick->CNTFG &= ~(1<<1);
}

/*******************************************************************************
* Function Name  : SYS_GetInfoSta
* Description    : ��ȡ��ǰϵͳ��Ϣ״̬
* Input          : i:
					refer to SYS_InfoStaTypeDef
* Return         : DISABLE  -  �ر�
				   ENABLE   -  ����
*******************************************************************************/
UINT8 SYS_GetInfoSta( SYS_InfoStaTypeDef i )
{
	return (R8_RST_BOOT_STAT&(1<<i));
}

/*******************************************************************************
* Function Name  : SYS_ResetExecute
* Description    : ִ��ϵͳ�����λ
* Input          : None
* Return         : None
*******************************************************************************/
void SYS_ResetExecute( void )
{
	R8_SAFE_ACCESS_SIG = 0x57; // enable safe access mode
	R8_SAFE_ACCESS_SIG = 0xa8;
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET | 0x40;
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : WWDG_ITCfg
* Description    : ���Ź���ʱ������ж�ʹ��
* Input          : s:
*                  DISABLE - ������ж�      
*                  ENABLE - ����ж�
* Return         : None
*******************************************************************************/
void  WWDG_ITCfg( UINT8 s )
{
	R8_SAFE_ACCESS_SIG = 0x57; // enable safe access mode
	R8_SAFE_ACCESS_SIG = 0xa8;
	if(s == DISABLE)		R8_RST_WDOG_CTRL=(R8_RST_WDOG_CTRL & (~RB_WDOG_INT_EN)) | 0x40;
	else 					R8_RST_WDOG_CTRL|=RB_WDOG_INT_EN | 0x40;
	R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : WWDG_ResetCfg
* Description    : ���Ź���ʱ����λ����
* Input          : s:
*                  DISABLE - �������λ      
*                  ENABLE - ���ϵͳ��λ
* Return         : None
*******************************************************************************/
void WWDG_ResetCfg( UINT8 s )
{
	R8_SAFE_ACCESS_SIG = 0x57; // enable safe access mode
	R8_SAFE_ACCESS_SIG = 0xa8;
	if(s == DISABLE)		R8_RST_WDOG_CTRL=(R8_RST_WDOG_CTRL & (~RB_WDOG_RST_EN)) | 0x40;
	else 					R8_RST_WDOG_CTRL|=RB_WDOG_RST_EN | 0x40;
	R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : WWDG_ClearFlag
* Description    : ������Ź��жϱ�־�����¼��ؼ���ֵҲ�����
* Input          : None
* Return         : None
*******************************************************************************/
void WWDG_ClearFlag( void )
{
	R8_SAFE_ACCESS_SIG = 0x57; // enable safe access mode
	R8_SAFE_ACCESS_SIG = 0xa8;
	R8_RST_WDOG_CTRL |= RB_WDOG_INT_FLAG | 0x40;
	R8_SAFE_ACCESS_SIG = 0;
}


#if( defined  DEBUG)
/*******************************************************************************
* Function Name  : _write
* Description    : Support Printf Function
* Input          : *buf: UART send Data.
*                  size: Data length
* Return         : size: Data length
*******************************************************************************/
int _write(int fd, char *buf, int size)
{
	int i;
	
	for(i=0; i<size; i++)
	{
#if  DEBUG == Debug_UART0
		while( R8_UART0_TFC == UART_FIFO_SIZE );
		R8_UART0_THR = *buf++;
#elif DEBUG == Debug_UART1 
		while( R8_UART1_TFC == UART_FIFO_SIZE );
		R8_UART1_THR = *buf++;
#elif DEBUG == Debug_UART2 
		while( R8_UART2_TFC == UART_FIFO_SIZE );
		R8_UART2_THR = *buf++;
#elif DEBUG == Debug_UART3  
		while( R8_UART3_TFC == UART_FIFO_SIZE );
		R8_UART3_THR = *buf++;		
#endif
	
	}
	
	return size;
}
#endif

