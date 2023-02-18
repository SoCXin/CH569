/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/07/31
 * Description 		 : UserOptionBytes�û����ú��������߽ӿڹرպ���
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"
#include "ISPEM569.h"

#define FREQ_SYS    80000000

/*******************************************************************************
 * @fn       DebugInit
 *
 * @brief    Initializes the UART1 peripheral.
 *
 * @param    baudrate: UART1 communication baud rate.
 *
 * @return   None
 */
void DebugInit(UINT32 baudrate)
{
    UINT32 x;
    UINT32 t = FREQ_SYS;

    x = 10 * t * 2 / 16 / baudrate;
    x = (x + 5) / 10;
    R8_UART1_DIV = 1;
    R16_UART1_DL = x;
    R8_UART1_FCR = RB_FCR_FIFO_TRIG | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN;
    R8_UART1_LCR = RB_LCR_WORD_SZ;
    R8_UART1_IER = RB_IER_TXD_EN;
    R32_PA_SMT |= (1 << 8) | (1 << 7);
    R32_PA_DIR |= (1 << 8);
}

/* Erase CH569-0xf7f9bf11 */

/*0x8000+0x14-32bit
 *User option byte define
 * [7-0] 8b' = ROM_CFG[0-7]
 *      -[3-0] reserved
 *      -[4] RESET_EN 0-�ⲿ��λ��ʹ�� 1-ʹ��
 *      -[7-5] 3b' 010 �̶�ֵ��Ч
 * [10-8] 3b' = ROM_CFG[31-29]
 *      -[8] LOCKUP_RST_EN
 *      -[10-9] USER_MEM
 * [11] 1b' = 1-Ĭ���������� 0-�ڶ���������
 * [19-12] 8b' = �û�������뱣��(4KΪ��λ)
 * [23-20] 4b' = 0101 �̶�ֵ��Ч
 * [31-24] 8b' = ~[7-0]��Ч
 *
 */

/* RESET_EN */
#define RESET_Enable              0x00000010
#define RESET_Disable             0xFFFFFFEF

/* LOCKUP_RST_EN */
#define LOCKUP_RST_Enable         0x20000000
#define LOCKUP_RST_Disable        0xDFFFFFFF

/* USER_MEM */
#define USER_MEM                  0x3FFFFFFF
#define USER_MEM_RAM32K_ROM96K    0x00000000
#define USER_MEM_RAM64K_ROM64K    0x40000000
#define USER_MEM_RAM96K_ROM32K    0x80000000

/* BOOT_PIN */
#define BOOT_PIN_PA5              0x00000800
#define BOOT_PIN_PA13             0xFFFFF7FF

/* FLASH_WRProt */
#define FLASH_WRProt              0xFFF00FFF
#define WRProt_Size               0x04        /* ��λ4KB */
#define FLASH_WRProt_Size_4KB     (WRProt_Size << 12)

/*******************************************************************************
 * @fn           UserOptionByteConfig
 *
 * @brief        Configure User Option Byte.
 *               (��ʹ�øú���������ʹ�ùٷ��ṩ��.S�ļ���ͬʱ���øú����������ϵ�����ߵ��Խӿ�Ĭ�Ϲر�)
 *
 * @param        RESET_EN:
 *                     ENABLE-�ⲿ��λ����ʹ��
 *                     DISABLE-��ʹ��
 *               LOCKUP_RST_EN:
 *                     ENABLE-�ں�LOCKUP��λϵͳʹ��
 *                     DISABLE-��ʹ��
 *               BOOT_PIN:
 *                     ENABLE-ʹ��Ĭ��boot��-PA5
 *                     DISABLE-ʹ��boot��-PA13
 *               USER_MEM_Set��RAM��ROM��������
 *                     USER_MEM_RAM32K_ROM96K
 *                     USER_MEM_RAM64K_ROM64K
 *                     USER_MEM_RAM96K_ROM32K
 *               FLASHProt_Size��д������С(��λ4K)
 *                     FLASH_WRProt_Size_4KB-
 *
 * @return       0: Success
 *               1: Err
 */
UINT8 UserOptionByteConfig(FunctionalState RESET_EN, FunctionalState LOCKUP_RST_EN, FunctionalState BOOT_PIN, UINT32 USER_MEM_Set, UINT32 FLASHProt_Size)
{
    UINT32 s, t;

    FLASH_ROMA_READ(0x14, &s, 4);

    if(s == 0xf7f9bf11)
    {
        s = 0;

        if(RESET_EN == ENABLE)
            s |= RESET_Enable;
        if(LOCKUP_RST_EN == ENABLE)
            s |= LOCKUP_RST_Enable;
        s |= USER_MEM_Set;
        s |= (1 << 6) | (5);

        /* bit[10-8] */
        t = ((s >> 21) & (7 << 8)) & (0x00000700);

        /* bit[7:0]-bit[31-24] */
        s |= 0xFFFFFF00;
        s &= ((~(s << 24)) | 0x000000FF); //��8λ ������Ϣȡ����

        /* bit[23-12] */
        s &= 0xFF0000FF;
        s |= FLASHProt_Size | (5 << 20);
        t |= s;

        /* bit[11] */
        if(BOOT_PIN == ENABLE)
            t |= (1 << 11);

        /*Write user option byte*/
        FLASH_ROMA_WRITE(0x14, &t, 4);

        /* Verify user option byte */
        FLASH_ROMA_READ(0x14, &s, 4);

        if(s == t)
            return 0;
        else
            return 1;
    }

    return 1;
}

/*******************************************************************************
 * @fn       Close_SWD
 *
 * @brief    �����ߵ��Խӿڣ���������ֵ���ֲ���.
 *
 * @return   0: Success
 *           1: Err
 */
UINT8 Close_SWD(void)
{
    UINT32 s, t;

    FLASH_ROMA_READ(0x14, &s, 4);

    if(s == 0xf7f9bf11)
    {
        FLASH_ROMA_READ(0x7EFFC - 0x8000, &s, 4); //CFG

        /* bit[10-8] */
        t = ((s >> 21) & (7 << 8)) & (0x00000700);

        /* bit[7:0]-bit[31-24] */
        s &= ~((1 << 5) | (1 << 7)); //���õ��Թ��ܣ� ����SPI��дFLASH
        s |= 0xFFFFFF00;
        s &= ((~(s << 24)) | 0x000000FF); //��8λ ������Ϣȡ����

        /* bit[7:0]-bit[31-24]-bit[10-8]-bit[23-12]=0x000*/
        s &= 0xFF0000FF;
        t |= s;

        /* bit[11] */
        FLASH_ROMA_READ(0x7EFFC - 0x8000 - 4, &s, 4); //CFG-4
        if(s & (1 << 1))
            s = (t | (1 << 11));
        else
            s = t;

        /*Write user option byte*/
        FLASH_ROMA_WRITE(0x14, &s, 4);

        /* Verify user option byte */
        FLASH_ROMA_READ(0x14, &t, 4);

        if(s == t)
            return 0;
        else
            return 1;
    }

    return 1;
}

UINT8 my_buffer[1024];

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main()
{
    UINT32 x;

    SystemInit(FREQ_SYS);
    Delay_Init(FREQ_SYS);

    DebugInit(115200);
    printf("Start @ChipID=%02X\r\n", R8_CHIP_ID);

    FLASH_ROMA_READ(0x7EFFC - 0x8000, &x, 4);
    printf("0x7EFFC-CFG-%08x\n", x);

    FLASH_ROMA_READ(0x7EFFC - 0x8000 - 4, &x, 4);
    printf("0x7EFF8-CFG-4-%08x\n", x);

    FLASH_ROMA_READ(0x14, &x, 4);
    printf("0x8014-%08x\n", x);

    {
        UINT8 p;

#if 0 /* �޸��û�����ֵ */
       p = UserOptionByteConfig(ENABLE, ENABLE, ENABLE, USER_MEM_RAM96K_ROM32K, FLASH_WRProt_Size_4KB);

#endif

#if 1 /* �ر����ߵ��Խӿ� */
        p = Close_SWD();

#endif

        if(p)
            printf("ERR\n");
        else
            printf("suc\n");
    }

    while(1);
}
