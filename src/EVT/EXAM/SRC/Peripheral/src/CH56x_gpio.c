/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_gpio.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
*******************************************************************************/

#include "CH56x_common.h"


/*******************************************************************************
* Function Name  : GPIOA_ModeCfg
* Description    : GPIOA�˿�����ģʽ����
* Input          : pin:  PA0-PA15
					GPIO_Pin_0 - GPIO_Pin_15
				   mode:
					GPIO_ModeIN_Floating       -  ��������/��������
					GPIO_ModeIN_PU_NSMT        -  ���������������
					GPIO_ModeIN_PD_NSMT        -  ���������������
					GPIO_ModeIN_PU_SMT         -  �����������ʩ��������
					GPIO_ModeIN_PD_SMT         -  �����������ʩ������
					GPIO_Slowascent_PP_8mA     -  ��б������������������� 8mA ����
					GPIO_Slowascent_PP_16mA    -  ��б������������������� 16mA ����
					GPIO_Highspeed_PP_8mA      -  ��������������������� 8mA ����
					GPIO_Highspeed_PP_16mA     -  ��������������������� 16mA ����
					GPIO_ModeOut_OP_8mA        -  ����������8mA
					GPIO_ModeOut_OP_16mA       -  ����������16mA
* Return         : None
*******************************************************************************/
void GPIOA_ModeCfg( UINT32 pin, GPIOModeTypeDef mode )
{	
    switch(mode)
    {
        case GPIO_ModeIN_Floating:
            R32_PA_PD &= ~pin;
            R32_PA_PU     &= ~pin;
            R32_PA_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PU_NSMT:
        	R32_PA_SMT &=~ pin;
            R32_PA_PD &= ~pin;
            R32_PA_PU     |= pin;
            R32_PA_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PD_NSMT:
        	R32_PA_SMT &=~ pin;
            R32_PA_PD |= pin;
            R32_PA_PU     &= ~pin;
            R32_PA_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PU_SMT:
        	R32_PA_SMT |= pin;
            R32_PA_PD &= ~pin;
            R32_PA_PU     |= pin;
            R32_PA_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PD_SMT:
        	R32_PA_SMT |= pin;
            R32_PA_PD |= pin;
            R32_PA_PU     &= ~pin;
            R32_PA_DIR    &= ~pin;
            break;

        case GPIO_Slowascent_PP_8mA:
        	R32_PA_SMT |= pin;
        	R32_PA_DRV &= ~pin;
            R32_PA_PD &= ~pin;
            R32_PA_DIR |= pin;
            break;

        case GPIO_Slowascent_PP_16mA:
        	R32_PA_SMT |= pin;
        	R32_PA_DRV |= pin;
            R32_PA_PD &= ~pin;
            R32_PA_DIR |= pin;
            break;

        case GPIO_Highspeed_PP_8mA:
        	R32_PA_SMT &= ~pin;
        	R32_PA_DRV &= ~pin;
            R32_PA_PD &= ~pin;
            R32_PA_DIR |= pin;
            break;

        case GPIO_Highspeed_PP_16mA:
        	R32_PA_SMT &= ~pin;
        	R32_PA_DRV |= pin;
            R32_PA_PD &= ~pin;
            R32_PA_DIR    |= pin;
            break;

        case GPIO_ModeOut_OP_8mA:
        	R32_PA_DRV &= ~pin;
            R32_PA_PD  |= pin;
            R32_PA_DIR    |= pin;
            break;

        case GPIO_ModeOut_OP_16mA:
        	R32_PA_DRV  |= pin;
            R32_PA_PD  |= pin;
            R32_PA_DIR    |= pin;
            break;

        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : GPIOB_ModeCfg
* Description    : GPIOB�˿�����ģʽ����
* Input          : pin:  PB0-PB15
					GPIO_Pin_0 - GPIO_Pin_15
				   mode:
					GPIO_ModeIN_Floating       -  ��������/��������
					GPIO_ModeIN_PU_NSMT        -  ���������������
					GPIO_ModeIN_PU_NSMT        -  ���������������
					GPIO_ModeIN_PU_SMT         -  �����������ʩ��������
					GPIO_ModeIN_PD_SMT         -  �����������ʩ������
					GPIO_Slowascent_PP_8mA     -  ��б������������������� 8mA ����
					GPIO_Slowascent_PP_16mA    -  ��б������������������� 16mA ����
					GPIO_Highspeed_PP_8mA      -  ��������������������� 8mA ����
					GPIO_Highspeed_PP_16mA     -  ��������������������� 16mA ����
					GPIO_ModeOut_OP_8mA        -  ����������8mA
					GPIO_ModeOut_OP_16mA       -  ����������16mA
* Return         : None
*******************************************************************************/
void GPIOB_ModeCfg( UINT32 pin, GPIOModeTypeDef mode )
{
    switch(mode)
    {
        case GPIO_ModeIN_Floating:
            R32_PB_PD &= ~pin;
            R32_PB_PU     &= ~pin;
            R32_PB_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PU_NSMT:
        	R32_PB_SMT &=~ pin;  
            R32_PB_PD &= ~pin;
            R32_PB_PU     |= pin;
            R32_PB_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PD_NSMT:
        	R32_PB_SMT &=~ pin;  
            R32_PB_PD |= pin;
            R32_PB_PU     &= ~pin;
            R32_PB_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PU_SMT:
        	R32_PB_SMT |= pin;  
            R32_PB_PD &= ~pin;
            R32_PB_PU     |= pin;
            R32_PB_DIR    &= ~pin;
            break;

        case GPIO_ModeIN_PD_SMT:
        	R32_PB_SMT |= pin;  
            R32_PB_PD |= pin;
            R32_PB_PU     &= ~pin;
            R32_PB_DIR    &= ~pin;
            break;

        case GPIO_Slowascent_PP_8mA:
        	R32_PB_SMT |= pin;  
        	R32_PB_DRV &= ~pin;
            R32_PB_PD &= ~pin;
            R32_PB_DIR |= pin;
            break;

        case GPIO_Slowascent_PP_16mA:
        	R32_PB_SMT |= pin;  
        	R32_PB_DRV |= pin;
            R32_PB_PD &= ~pin;
            R32_PB_DIR |= pin;
            break;

        case GPIO_Highspeed_PP_8mA:
        	R32_PB_SMT &= ~pin;  
        	R32_PB_DRV &= ~pin;
            R32_PB_PD &= ~pin;
            R32_PB_DIR |= pin;
            break;

        case GPIO_Highspeed_PP_16mA:
        	R32_PB_SMT &= ~pin;  
        	R32_PB_DRV |= pin;
            R32_PB_PD &= ~pin;
            R32_PB_DIR    |= pin;
            break;

        case GPIO_ModeOut_OP_8mA:
        	R32_PB_DRV &= ~pin;
            R32_PB_PD  |= pin;
            R32_PB_DIR    |= pin;
            break;

        case GPIO_ModeOut_OP_16mA:
        	R32_PB_DRV |= pin;
            R32_PB_PD  |= pin;
            R32_PB_DIR    |= pin;
            break;

        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : GPIOA_ITModeCfg
* Description    : GPIOA�����ж�ģʽ����
* Input          : pin:  PA2-PA4
				   mode:
					GPIO_ITMode_LowLevel   -  �͵�ƽ����
					GPIO_ITMode_HighLevel  -  �ߵ�ƽ����
					GPIO_ITMode_FallEdge   -  �½��ش���
					GPIO_ITMode_RiseEdge   -  �����ش���
* Return         : None
*******************************************************************************/
void GPIOA_ITModeCfg( UINT32 pin, GPIOITModeTpDef mode )
{
    switch( mode )
    {
        case GPIO_ITMode_LowLevel:		// �͵�ƽ����
        	R32_PA_DIR &= ~pin;
        	R8_GPIO_INT_MODE &= ~(pin>>2);
        	R8_GPIO_INT_POLAR &= ~(pin>>2);
            break;

        case GPIO_ITMode_HighLevel:		// �ߵ�ƽ����
        	R32_PA_DIR &= ~pin;
        	R8_GPIO_INT_MODE &= ~(pin>>2);
            R8_GPIO_INT_POLAR |= (pin>>2);
            break;

        case GPIO_ITMode_FallEdge:		// �½��ش���
        	R32_PA_DIR &= ~pin;
        	R8_GPIO_INT_MODE |= (pin>>2);
            R8_GPIO_INT_POLAR &= ~(pin>>2);
            break;

        case GPIO_ITMode_RiseEdge:		// �����ش���
        	R32_PA_DIR &= ~pin;
        	R8_GPIO_INT_MODE |= (pin>>2);
            R8_GPIO_INT_POLAR |= (pin>>2);
            break;

        default :
            break;
    }
    R8_GPIO_INT_FLAG = (pin>>2);
    R8_GPIO_INT_ENABLE |= (pin>>2);
}

/*******************************************************************************
* Function Name  : GPIOB_ITModeCfg
* Description    : GPIOB�����ж�ģʽ����
* Input          : pin:  3,4,11,12,15
				   mode:
					GPIO_ITMode_LowLevel   -  �͵�ƽ����
					GPIO_ITMode_HighLevel  -  �ߵ�ƽ����
					GPIO_ITMode_FallEdge   -  �½��ش���
					GPIO_ITMode_RiseEdge   -  �����ش���
* Return         : None
*******************************************************************************/
void GPIOB_ITModeCfg( UINT32 pin, GPIOITModeTpDef mode )
{
    switch( mode )
    {
        case GPIO_ITMode_LowLevel:		// �͵�ƽ����
          if(pin==3)
          {
        	R32_PB_DIR &= ~(1<<3);
        	R8_GPIO_INT_MODE &= ~(1<<3);
        	R8_GPIO_INT_POLAR &= ~(1<<3);
        	R8_GPIO_INT_FLAG = (1<<3);
            R8_GPIO_INT_ENABLE |= (1<<3);
          }
          else if(pin==4)
          {
         	R32_PB_DIR &= ~(1<<4);
        	R8_GPIO_INT_MODE &= ~(1<<4);
        	R8_GPIO_INT_POLAR &= ~(1<<4);
        	R8_GPIO_INT_FLAG = (1<<4);
            R8_GPIO_INT_ENABLE |= (1<<4);
          }
          else if(pin==11)
          {
         	R32_PB_DIR &= ~(1<<11);
        	R8_GPIO_INT_MODE &= ~(1<<5);
        	R8_GPIO_INT_POLAR &= ~(1<<5);
        	R8_GPIO_INT_FLAG = (1<<5);
            R8_GPIO_INT_ENABLE |= (1<<5);
          }
          else if(pin==12)
          {
         	R32_PB_DIR &= ~(1<<12);
        	R8_GPIO_INT_MODE &= ~(1<<6);
        	R8_GPIO_INT_POLAR &= ~(1<<6);
        	R8_GPIO_INT_FLAG = (1<<6);
            R8_GPIO_INT_ENABLE |= (1<<6);
          }
          else if(pin==15)
          {
         	R32_PB_DIR &= ~(1<<15);
        	R8_GPIO_INT_MODE &= ~(1<<7);
        	R8_GPIO_INT_POLAR &= ~(1<<7);
        	R8_GPIO_INT_FLAG = (1<<7);
            R8_GPIO_INT_ENABLE |= (1<<7);
          }
          break;

       case GPIO_ITMode_HighLevel:		// �ߵ�ƽ����
          if(pin==3)
          {
        	R32_PB_DIR &= ~(1<<3);
        	R8_GPIO_INT_MODE &= ~(1<<3);
        	R8_GPIO_INT_POLAR |= (1<<3);
        	R8_GPIO_INT_FLAG = (1<<3);
            R8_GPIO_INT_ENABLE |= (1<<3);
          }
          else if(pin==4)
          {
         	R32_PB_DIR &= ~(1<<4);
        	R8_GPIO_INT_MODE &= ~(1<<4);
        	R8_GPIO_INT_POLAR |= (1<<4);
        	R8_GPIO_INT_FLAG = (1<<4);
            R8_GPIO_INT_ENABLE |= (1<<4);
          }
          else if(pin==11)
          {
         	R32_PB_DIR &= ~(1<<11);
        	R8_GPIO_INT_MODE &= ~(1<<5);
        	R8_GPIO_INT_POLAR |= (1<<5);
        	R8_GPIO_INT_FLAG = (1<<5);
            R8_GPIO_INT_ENABLE |= (1<<5);
          }
          else if(pin==12)
          {
         	R32_PB_DIR &= ~(1<<12);
        	R8_GPIO_INT_MODE &= ~(1<<6);
        	R8_GPIO_INT_POLAR |= (1<<6);
        	R8_GPIO_INT_FLAG = (1<<6);
            R8_GPIO_INT_ENABLE |= (1<<6);
          }
          else if(pin==15)
          {
         	R32_PB_DIR &= ~(1<<15);
        	R8_GPIO_INT_MODE &= ~(1<<7);
        	R8_GPIO_INT_POLAR |= (1<<7);
        	R8_GPIO_INT_FLAG = (1<<7);
            R8_GPIO_INT_ENABLE |= (1<<7);
          }
          break;

          case GPIO_ITMode_FallEdge:		// �½��ش���
              if(pin==3)
                    {
                  	R32_PB_DIR &= ~(1<<3);
                  	R8_GPIO_INT_MODE |= (1<<3);
                  	R8_GPIO_INT_POLAR &= ~(1<<3);
                  	R8_GPIO_INT_FLAG = (1<<3);
                      R8_GPIO_INT_ENABLE |= (1<<3);
                    }
                    else if(pin==4)
                    {
                   	R32_PB_DIR &= ~(1<<4);
                  	R8_GPIO_INT_MODE |= (1<<4);
                  	R8_GPIO_INT_POLAR &= ~(1<<4);
                  	R8_GPIO_INT_FLAG = (1<<4);
                      R8_GPIO_INT_ENABLE |= (1<<4);
                    }
                    else if(pin==11)
                    {
                   	R32_PB_DIR &= ~(1<<11);
                  	R8_GPIO_INT_MODE |= (1<<5);
                  	R8_GPIO_INT_POLAR &= ~(1<<5);
                  	R8_GPIO_INT_FLAG = (1<<5);
                      R8_GPIO_INT_ENABLE |= (1<<5);
                    }
                    else if(pin==12)
                    {
                   	R32_PB_DIR &= ~(1<<12);
                  	R8_GPIO_INT_MODE |= (1<<6);
                  	R8_GPIO_INT_POLAR &= ~(1<<6);
                  	R8_GPIO_INT_FLAG = (1<<6);
                      R8_GPIO_INT_ENABLE |= (1<<6);
                    }
                    else if(pin==15)
                    {
                   	R32_PB_DIR &= ~(1<<15);
                  	R8_GPIO_INT_MODE |= (1<<7);
                  	R8_GPIO_INT_POLAR &= ~(1<<7);
                  	R8_GPIO_INT_FLAG = (1<<7);
                      R8_GPIO_INT_ENABLE |= (1<<7);
                    }
              break;


       case GPIO_ITMode_RiseEdge:		// �����ش���
             if(pin==3)
                    {
                     R32_PB_DIR &= ~(1<<3);
                     R8_GPIO_INT_MODE |= (1<<3);
                     R8_GPIO_INT_POLAR |= (1<<3);
                     R8_GPIO_INT_FLAG = (1<<3);
                     R8_GPIO_INT_ENABLE |= (1<<3);
                    }
                    else if(pin==4)
                    {
                    R32_PB_DIR &= ~(1<<4);
                    R8_GPIO_INT_MODE |= (1<<4);
                    R8_GPIO_INT_POLAR |= (1<<4);
                    R8_GPIO_INT_FLAG = (1<<4);
                    R8_GPIO_INT_ENABLE |= (1<<4);
                    }
                    else if(pin==11)
                    {
                    R32_PB_DIR &= ~(1<<11);
                    R8_GPIO_INT_MODE |= (1<<5);
                    R8_GPIO_INT_POLAR |= (1<<5);
                    R8_GPIO_INT_FLAG = (1<<5);
                    R8_GPIO_INT_ENABLE |= (1<<5);
                    }
                    else if(pin==12)
                    {
                    R32_PB_DIR &= ~(1<<12);
                    R8_GPIO_INT_MODE |= (1<<6);
                    R8_GPIO_INT_POLAR |= (1<<6);
                    R8_GPIO_INT_FLAG = (1<<6);
                    R8_GPIO_INT_ENABLE |= (1<<6);
                    }
                    else if(pin==15)
                    {
                    R32_PB_DIR &= ~(1<<15);
                    R8_GPIO_INT_MODE |= (1<<7);
                    R8_GPIO_INT_POLAR |= (1<<7);
                    R8_GPIO_INT_FLAG = (1<<7);
                    R8_GPIO_INT_ENABLE |= (1<<7);
                    }
             break;

        default :
        break;
    }
}

/*******************************************************************************
* Function Name  : GPIOPinRemap
* Description    : ���蹦������ӳ��
* Input          : s:
					ENABLE  - ����ӳ��
					DISABLE - Ĭ������
				   perph:
					RB_PIN_UART0  -  RXD0:  PB5 ->  PA5
					              -  TXD0:  PB6 ->  PA6
					RB_PIN_TMR2	  -  TMR2/PWM6/CAP2:  PA4 ->  PB3
					RB_PIN_TMR1	  -  TMR1/PWM5/CAP1:  PB15 ->  PB0
					RB_PIN_MII	  -  Ethernet:  RMII ->  RGMII
* Return         : None
*******************************************************************************/
void GPIOPinRemap( UINT8 s, UINT16 perph )
{
    if( s )     R8_PIN_ALTERNATE |= perph;
    else        R8_PIN_ALTERNATE &= ~perph;
}

/*******************************************************************************
* Function Name  : GPIOMco
* Description    : GPIO MCOģʽ����
* Input          :  ENABLE\DISABLE
                    mode:
				   	MCO_125,
	                MCO_25,
	                MCO_2d5,
* Return         : None
*******************************************************************************/
void GPIOMco( UINT8 s, UINT16 freq )
{
	if(s)
	{
		R8_SAFE_ACCESS_SIG = 0x57;
		R8_SAFE_ACCESS_SIG = 0xa8;

		R8_CLK_MOD_AUX = 0x10;
		R8_CLK_MOD_AUX |=0x01;
		R8_CLK_MOD_AUX |= freq;
	}
	else R8_CLK_MOD_AUX &= ~(1<<4);
}




