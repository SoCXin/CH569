/********************************** (C) COPYRIGHT *******************************
* File Name          : debug.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
*******************************************************************************/

#include "debug.h"
#include "CH56xSFR.h"

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
		while( R8_UART3_TFC == UART_FIFO_SIZE );
		R8_UART3_THR = *buf++;
	}
	return size;
}



