/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56U.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
*******************************************************************************/

#include "CH56xSFR.h"
#include "CH56U.h"
#include "core_riscv.h"

/*******************************************************************************
* Function Name  : mDelayuS
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void mDelayuS( UINT16 n )
{
	unsigned char i,s;

	while ( n ) {  // total = 80/6= 13 Fsys cycles, 1uS @Fsys=80MHz
		for(i=40;i!=0;i--)s = R8_PA_OUT_0 ;//6clk
		n--;
	}
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void mDelaymS( UINT16 n )
{
	unsigned char i;

	while ( n ) {  // total = 80/6= 13 Fsys cycles, 1uS @Fsys=80MHz
		mDelayuS(400);
		n--;
	}
}

