/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_pwr.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
*******************************************************************************/

#include "CH56x_common.h"


/*******************************************************************************
* Function Name  : PWR_PeriphClkCfg
* Description    : ����ʱ�ӿ���λ
* Input          : s:
                    ENABLE  - ������ʱ��
                    DISABLE - �ر�����ʱ��
                   perph:
                    please refer to Peripher CLK control bit define
* Return         : None
*******************************************************************************/
void PWR_PeriphClkCfg( UINT8 s, UINT16 perph )
{
    if( s == DISABLE )
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R32_SLEEP_CONTROL |= perph;
    }
    else
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R32_SLEEP_CONTROL &= ~perph;
    }
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : PWR_PeriphWakeUpCfg
* Description    : ˯�߻���Դ����
* Input          : s:
                    ENABLE  - �򿪴�����˯�߻��ѹ���
                    DISABLE - �رմ�����˯�߻��ѹ���
                   perph:
                   RB_SLP_USBHS_WAKE    -  USB2.0Ϊ����Դ
                   RB_SLP_USBSS_WAKE    -  USB3.0Ϊ����Դ
                   RB_SLP_GPIO_WAKE     -  GPIOΪ����Դ
                   RB_SLP_ETH_WAKE      -  ETHΪ����Դ
                    ALL                 -  ��������
* Return         : None
*******************************************************************************/
void PWR_PeriphWakeUpCfg( UINT8 s, UINT16 perph )
{
    if( s == DISABLE )
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R8_SLP_WAKE_CTRL &= ~perph;
    }
    else
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R8_SLP_WAKE_CTRL |= perph;
    }
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : LowPower_Idle
* Description    : �͹���-Idleģʽ
* Input          : None
* Return         : None
*******************************************************************************/
void LowPower_Idle( void )
{

	PFIC->SCTLR &= ~1<<2;				// �����ں�PFIC_SCTLR�Ĵ�����SleepDeep�ֶ�Ϊ0
    __WFI();                            // ���û���������ִ��__WFI()

}

/*******************************************************************************
* Function Name  : LowPower_Halt
* Description    : �͹���-Haltģʽ
* Input          : None
* Return         : None
*******************************************************************************/
void LowPower_Halt( void )
{

	PFIC->SCTLR |= 1<<2;                      // �����ں�PFIC_SCTLR�Ĵ�����SleepDeep�ֶ�Ϊ1
	R8_SLP_POWER_CTRL |= RB_SLP_USBHS_PWRDN;  // RB_SLP_USBHS_PWRDN��1
	__WFI();                                  // ���û���������ִ��__WFI()
}

/*******************************************************************************
* Function Name  : LowPower_Sleep
* Description    : �͹���-Sleepģʽ
* Input          : None
* Return         : None
*******************************************************************************/
void LowPower_Sleep( void )
{

	PFIC->SCTLR |= 1<<2;                      // �����ں�PFIC_SCTLR�Ĵ�����SleepDeep�ֶ�Ϊ1
	R8_SLP_POWER_CTRL |= RB_SLP_USBHS_PWRDN;  // RB_SLP_USBHS_PWRDN��1
	R8_SLP_WAKE_CTRL &= ~RB_SLP_USBSS_WAKE;   // RB_SLP_USBSS_WAKE��0
	__WFI();                                  // ���û���������ִ��__WFI()

}




