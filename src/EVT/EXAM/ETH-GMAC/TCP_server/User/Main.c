/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.1
* Date               : 2021/07/31
* Description 		 : CH565/569 TCP/IP��������ʾ���̣�ʹ����̫��ʱ����Ƶ���õ���60MHz
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

/*-----------------------------------ͷ�ļ�����--------------------------------------*/
#include "CH56x_common.h"    /* CH565����ͷ�ļ�  */
#include "ethernet_driver.h" /* ��̫������  */
#include "ethernet_config.h" /* ��̫������  */
#include <CH569Net_lib.h>    /* Э��ջ��ͷ�ļ� */
#include "ISPEM569.h"        /* FLASH��ͷ�ļ�  */
#include "timer.h"           /* Э��ջʱ����Ҫ�Ķ�ʱ����������ʱ�� */
#include "macros.h"          /* ����ʱ�õ��ĺ�  */

/*---------------------------------------��------------------------------------------*/
#define KEEPLIVE_ENABLE    1  /* �Ƿ�����KEEPLIVE���� */
#define create_a_socket    1  /* �Ƿ���һ��socket */

/*------------------------------------ȫ�ֱ���----------------------------------------*/

/* TCP/IPЭ��ջ���ز��� */
UINT8 local_mac[6] = {
    0x84,
    0xc2,
    0xe4,
    0x01,
    0x02,
    0x03,
};                                         /* ����MAC��ַ�����ں�������ʵ�ʵ�MAC��ַ  */
UINT8 CH569IPAddr[4] = {192, 168, 1, 200}; /* Ŀ��IP��ַ */
UINT8 GatewayIp[4] = {192, 168, 1, 1};     /* ���ص�ַ  */
UINT8 SubMaskIp[4] = {255, 255, 225, 0};   /* ��������   */
#if create_a_socket
UINT8 DESIP[4] = {192, 168, 1, 100}; /* Ŀ��IP */
#endif

/* ���ʹ��DHCP������ */
#ifdef USE_DHCP
UINT8 dhcp_source_ip[4] = {0, 0, 0, 0};
  #define init_ip    dhcp_source_ip
#else
  #define init_ip    CH569IPAddr
#endif

UINT8 SocketId;                                             /* ����socket���������Բ��ö��� */
UINT8 SocketRecvBuf[CH569NET_MAX_SOCKET_NUM][RECE_BUF_LEN]; /* socket���ջ����� */
UINT8 MyBuf[RECE_BUF_LEN];                                  /* ����һ����ʱ������ */

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

/*******************************************************************************
 * @fn        net_initkeeplive
 *
 * @brief     keeplive��ʼ��
 *
 * @return    None
 */
#ifdef KEEPLIVE_ENABLE
void net_initkeeplive(void)
{
    struct _KEEP_CFG klcfg;

    klcfg.KLIdle = 72000000; /* ���� */
    klcfg.KLIntvl = 75000;   /* ��� */
    klcfg.KLCount = 9;       /* ���� */
    CH569NET_ConfigKeepLive(&klcfg);
}
#endif

/*******************************************************************************
 * @fn       CH563NET_CreatUpdSocket
 *
 * @brief    ����һ�� UDPsocket
 *
 * @return   None
 */
void CH563NET_CreatUpdSocket(void)
{
    UINT8    i;
    SOCK_INF TmpSocketInf; /* ������ʱsocket���� */

    memset((void *)&TmpSocketInf, 0, sizeof(SOCK_INF));     /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
    memcpy((void *)TmpSocketInf.IPAddr, DESIP, 4);          /* ����Ŀ��IP��ַ */
    TmpSocketInf.DesPort = 1000;                            /* ����Ŀ�Ķ˿� */
    TmpSocketInf.SourPort = 2000;                           /* ����Դ�˿� */
    TmpSocketInf.ProtoType = PROTO_TYPE_UDP;                /* ����socekt���� */
    TmpSocketInf.RecvStartPoint = (UINT32)SocketRecvBuf[0]; /* ���ý��ջ������Ľ��ջ����� */
    TmpSocketInf.RecvBufLen = RECE_BUF_LEN;                 /* ���ý��ջ������Ľ��ճ��� */
    i = CH569NET_SocketCreat(&SocketId, &TmpSocketInf);     /* ����socket�������ص�socket����������SocketId�� */
    mStopIfError(i);                                        /* ������ */
}

/*******************************************************************************
 * @fn            CH569NET_LibInit
 *
 * @brief         ���ʼ������
 *
 * @param ip      ip - ��ַָ��
 *                gwip - ����ip��ַָ��
 *                mask - ����ָ��
 *                macaddr - MAC��ַָ��
 *
 * @return        ִ��״̬
 */
UINT8 CH569NET_LibInit(UINT8 *ip, UINT8 *gwip, UINT8 *mask, UINT8 *macaddr)
{
    UINT8             i;
    struct _CH569_CFG cfg;

    if(CH569NET_GetVer() != CH569NET_LIB_VER)
        return 0xfc;                /* ��ȡ��İ汾�ţ�����Ƿ��ͷ�ļ�һ�� */
    CH569NETConfig = LIB_CFG_VALUE; /* ��������Ϣ���ݸ�������ñ��� */
    cfg.RxBufSize = /*RX_BUF_SIZE*/ 1520;
    cfg.TCPMss = CH569NET_TCP_MSS;
    cfg.HeapSize = CH569_MEM_HEAP_SIZE;
    cfg.ARPTableNum = CH569NET_NUM_ARP_TABLE;
    cfg.MiscConfig0 = CH569NET_MISC_CONFIG0;

    CH569NET_ConfigLIB(&cfg);
    i = CH569NET_Init(ip, gwip, mask, macaddr);
#ifdef KEEPLIVE_ENABLE
    net_initkeeplive(); /* �����Զ���ı��ʱ����������Ҫ�ڳ�ʼ����֮���趨 */
#endif
    return (i); /* ���ʼ�� */
}

/*******************************************************************************
 * @fn        CH569NET_HandleSockInt
 *
 * @brief     Socket�жϴ�����
 *
 * @param     sockeid - socket����
 *            initstat - �ж�״̬
 *
 * @return     None
 */
void CH569NET_HandleSockInt(UINT8 sockeid, UINT8 initstat)
{
    UINT32 len;
    UINT32 totallen;
#if create_a_socket
    UINT8 *p = MyBuf, i;
    char   rc;
#endif

    printf("CH569NET_HandleSockInt:%02x.\n", initstat);
    if(initstat & SINT_STAT_RECV) /* �����ж� */
    {
#if create_a_socket
        len = CH569NET_SocketRecvLen(sockeid, NULL); /* ��ѯ����*/
        totallen = len;
        CH569NET_SocketRecv(sockeid, MyBuf, &len); /* �����ջ����������ݶ���MyBuf�� */
  #if 1
        while(1)
        {
            i++;
            len = totallen;
            rc = CH569NET_SocketSend(sockeid, p, &len);
            if(rc != 0) /* ��MyBuf�е����ݷ��� */
            {
                printf("---------------Send error:%x.---------------------\n", rc);
                break;
            }
            totallen -= len; /* ���ܳ��ȼ�ȥ�Լ�������ϵĳ��� */
            p += len;        /* ��������ָ��ƫ��*/
            if(i >= 10)
                break;
            if(totallen)
                continue; /* �������δ������ϣ����������*/
            break;        /* ������ϣ��˳� */
        }
  #endif
#endif
    }
    if(initstat & SINT_STAT_CONNECT) /* TCP�����ж� */
    {                                /* �������жϱ�ʾTCP�Ѿ����ӣ����Խ����շ����� */
        printf("TCP Connect Success\n");

        CH569NET_ModifyRecvBuf(sockeid, (UINT32)SocketRecvBuf[sockeid], RECE_BUF_LEN);
#if KEEPLIVE_ENABLE
        CH569NET_SocketSetKeepLive(sockeid, 1);
#endif
    }
    if(initstat & SINT_STAT_DISCONNECT) /* TCP�Ͽ��ж� */
    {                                   /* �������жϣ�CH569���ڲ��Ὣ��socket�������Ϊ�ر�*/
        printf("TCP Disconnect\n");     /* Ӧ������������´������� */
    }
    if(initstat & SINT_STAT_TIM_OUT) /* TCP��ʱ�ж� */
    {                                /* �������жϣ�CH569���ڲ��Ὣ��socket�������Ϊ�ر�*/
        printf("TCP Timout\n");      /* Ӧ������������´������� */
    }
}

/*******************************************************************************
 * @fn        CH569NET_HandleGloableInt
 *
 * @brief     ȫ���жϴ�����
 *
 * @return    None
 */
void CH569NET_HandleGlobalInt(void)
{
    UINT8 initstat;
    UINT8 i;
    UINT8 socketinit;

    initstat = CH569NET_GetGlobalInt(); /* ��ȫ���ж�״̬����� */
    if(initstat & GINT_STAT_UNREACH)    /* ���ɴ��ж� */
    {
        printf("UnreachCode ��%d\n", CH569Inf.UnreachCode);   /* �鿴���ɴ���� */
        printf("UnreachProto ��%d\n", CH569Inf.UnreachProto); /* �鿴���ɴ�Э������ */
        printf("UnreachPort ��%d\n", CH569Inf.UnreachPort);   /* ��ѯ���ɴ�˿� */
    }
    if(initstat & GINT_STAT_IP_CONFLI) /* IP��ͻ�ж� */
    {
        printf("Error:IP conflict.\n");
    }
    if(initstat & GINT_STAT_SOCKET) /* Socket�ж� */
    {
        for(i = 0; i < CH569NET_MAX_SOCKET_NUM; i++)
        {
            socketinit = CH569NET_GetSocketInt(i); /* ��socket�жϲ����� */
            if(socketinit)
                CH569NET_HandleSockInt(i, socketinit); /* ������ж������� */
        }
    }
}

/*******************************************************************************
 * @fn       CH569NET_CreatTcpSocket
 *
 * @brief    ����TCP Server socket
 *
 * @return   None
 */
void CH569NET_CreatTcpSocket(void)
{
    UINT8    i;
    SOCK_INF TmpSocketInf; /* ������ʱsocket���� */

    memset((void *)&TmpSocketInf, 0, sizeof(SOCK_INF)); /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */

    TmpSocketInf.SourPort = 2000;                        /* ����Դ�˿� */
    TmpSocketInf.ProtoType = PROTO_TYPE_TCP;             /* ����socekt���� */
    TmpSocketInf.RecvStartPoint = (UINT32)SocketRecvBuf; /* ���ý��ջ������Ľ��ջ����� */
    TmpSocketInf.RecvBufLen = RECE_BUF_LEN;              /* ���ý��ջ������Ľ��ճ��� */
    i = CH569NET_SocketCreat(&SocketId, &TmpSocketInf);  /* ����socket�������ص�socket����������SocketId�� */
    mStopIfError(i);                                     /* ������ */
    i = CH569NET_SocketListen(SocketId);
    mStopIfError(i); /* ������ */
}

/*******************************************************************************
 * @fn        DHCP_CALLBACK
 *
 * @brief     DHCP�ص�����
 *
 * @param     rc - ״̬
 *            p - ����ָ��
 *
 * @return    ִ��״̬
 */
UINT8 DHCP_CALLBACK(UINT8 rc, void *p)
{
    UINT8 *ptr = p;
    UINT8  ip[4], mask[4], gateway[4];

    if(rc != 0)
    {
        printf("DHCP failed!\n");
        reset_net_para(CH569IPAddr, SubMaskIp, GatewayIp);
        return 1;
    }
    printf("DHCP success.\n");
    printf("ip:%d.%d.%d.%d\ngate:%d.%d.%d.%d\nmask:%d.%d.%d.%d\ndns:%d.%d.%d.%d\ndns2:%d.%d.%d.%d\n",
           *ptr++, *ptr++, *ptr++, *ptr++,
           *ptr++, *ptr++, *ptr++, *ptr++,
           *ptr++, *ptr++, *ptr++, *ptr++,
           *ptr++, *ptr++, *ptr++, *ptr++, *ptr++, *ptr++, *ptr++, *ptr);
    memcpy(ip, ptr - 19, 4);
    memcpy(mask, ptr - 11, 4);
    memcpy(gateway, ptr - 15, 4);
    reset_net_para(ip, mask, gateway);
    return 0;
}

/*******************************************************************************
 * @fn       register_if_fn
 *
 * @brief    ΪЭ��ջע����̫���ӿں���
 *
 * @return   None
 */
void register_if_fn(void)
{
    ethernet_if.get_send_ptr = Get_TxBuff_Addr;
    ethernet_if.rece_fn = mac_rece;
    ethernet_if.send_fn = mac_send;
}

/*******************************************************************************
 * @fn         tcp_ip_stack_mian_process
 *
 * @brief      Э��ջ���̣���Ҫ����ѭ���в��ϵ���
 *
 * @return     None
 */
void tcp_ip_stack_main_process(void)
{
    CH569NET_MainTask(); /* CH563NET��������������Ҫ����ѭ���в��ϵ��� */
    if(CH569NET_QueryGlobalInt())
        CH569NET_HandleGlobalInt(); /* ��ѯ�жϣ�������жϣ������ȫ���жϴ����� */
}

/*******************************************************************************
 * @fn       get_unique_mac
 *
 * @brief    ��ȡΨһMAC��ַ�ĺ���
 *
 * @param    �洢MAC��6�ֽڿռ��ָ��
 *
 * @return   None
 */
void get_unique_mac(UINT8 *MAC_PTR)
{
    UINT8 rc, buf[8];

    rc = FLASH_ROMA_READ((0x7FFE4 - 0x8000), buf, 8);
    mStopIfError(rc);
    for(rc = 0; rc < 6; rc++)
        MAC_PTR[0 + rc] = buf[5 - rc];
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
    UINT8  rc; /* ״̬ */
    UINT8 *read_ptr, **p = &read_ptr;
    UINT16 read_length, i;
    UINT32 DIFF = 0;

    /* ϵͳ��ʼ�� */
    SystemInit(FREQ_SYS);
    Delay_Init(FREQ_SYS);
    /* ���ô��ڵ��� */
    DebugInit(921600);
    get_unique_mac(local_mac);
    printf("Start @ChipID=%02X\r\n", R8_CHIP_ID);
    printf("WCH TCP server demo.\n");
    printf("System frequency is :%d Hz.Compiled at %s,%s\n", FREQ_SYS, __TIME__, __DATE__);
    printf("CH569 MAC address is:%02x:%02x:%02x:%02x:%02x:%02x\n", local_mac[0], local_mac[1], local_mac[2], local_mac[3], local_mac[4], local_mac[5]);

    /* MAC��ʼ������Ҫ��֤����/�����Ѿ����� */
    ETH_init();
    /* ��ʱ����ʼ��,ÿ10�����һ���жϷ����� */
    TMR0_init(FREQ_SYS / 100);
    tmr1_init();
#ifdef USE_RTL8211FS
    /* ���÷���ʱ���ӳ٣��ڽ��ղ࣬RGMII��ʱ����Ҫ�������ӳ�1/4���ڡ��û���Ҫ�����Լ���PCB�����֤ */
    RGMII_TXC_Delay(0, 2); /* RGMII�����ӳٺ���λ���� */
#endif
#if USE_ETH_PHY_INTERRUPT
    phy_int_cheak_init();
#endif
    mDelaymS(2000);
#if 0 /* ��11�¸İ�֮ǰ��оƬ��Ҫ������һ��  */
	Check_TxDes();                                                              /* �������������  */
#endif
    register_if_fn();                                               /* �����ע��MAC�ӿں��� */
    i = CH569NET_LibInit(init_ip, GatewayIp, SubMaskIp, local_mac); /* ���ʼ�� */
    mStopIfError(i);                                                /* ����ϲ��Ĵ���  */

    Enabl_MAC_RECV(); /* ʹ�ܽ��� */

#if create_a_socket
    CH569NET_CreatTcpSocket(); /* ����TCP Socket */
#endif

    printf("enter main loop!\r\n");

    while(1)
    {
        tcp_ip_stack_main_process();
    }
}

/*******************************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   ����Ӳ������
 *
 * @return  None
 */
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void)
{
    printf("mepc=%08x\r\n", read_csr(mepc));
    printf("mcause=%08x\r\n", read_csr(mcause));
    printf("mtval=%08x\r\n", read_csr(mtval));
    while(1);
}
