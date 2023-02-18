/********************************** (C) COPYRIGHT *******************************
* File Name          : CH569Net_lib.h
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
*******************************************************************************/
/*
 * ���ļ�ֻ�ܱ��û��������
 *  */
#ifndef   _CH569NETLIB_H__
#define   _CH569NETLIB_H__

#ifndef UINT8
typedef unsigned char UINT8;
#endif

#ifndef UINT16
typedef unsigned short UINT16;
#endif

#ifndef UINT32
typedef unsigned long UINT32;
#endif

#define CH569NET_LIB_VER                      0x02                              /* �汾�ţ�0x02 */

#ifndef CH569NET_MAX_SOCKET_NUM
#define CH569NET_MAX_SOCKET_NUM               4                                 /* Socket�ĸ������û��������ã�Ĭ��Ϊ4��Socket,���Ϊ32 */
#endif

#define CH569NET_TCP_MSS                      536

#ifndef CH569NET_PING_ENABLE
#define CH569NET_PING_ENABLE                  TRUE                              /* Ĭ��PING���� */
#endif                                                                          /* PINGʹ�� */

#ifndef TCP_RETRY_COUNT
#define TCP_RETRY_COUNT                       20                                /* TCP���Դ�����λ��Ϊ5λ*/
#endif

#ifndef TCP_RETRY_PERIOD
#define TCP_RETRY_PERIOD                      2                                /* TCP�������ڣ���λΪ50MS��λ��Ϊ5λ */
#endif

#ifndef CH569NETTIMEPERIOD
#define CH569NETTIMEPERIOD                    10                                /* ��ʱ�����ڣ���λMs,���ô���500 */
#endif

#ifndef SOCKET_SEND_RETRY
#define SOCKET_SEND_RETRY                     1                                 /* Ĭ�Ϸ������� */
#endif

#define LIB_CFG_VALUE                         ((SOCKET_SEND_RETRY << 25) |\
                                              (/*MAC_INT_TYPE*/1 << 24) |\
                                              (TCP_RETRY_PERIOD << 19) |\
                                              (TCP_RETRY_COUNT << 14) |\
                                              (CH569NET_PING_ENABLE << 13) |\
                                              (/*TX_QUEUE_ENTRIES*/2 << 9) |\
                                              (/*RX_QUEUE_ENTRIES*/8 << 5) |\
                                              (CH569NET_MAX_SOCKET_NUM))
#ifndef MISCE_CFG0_TCP_SEND_COPY
#define MISCE_CFG0_TCP_SEND_COPY              1                                 /* TCP���ͻ��������� */
#endif

#ifndef MISCE_CFG0_TCP_RECV_COPY
#define MISCE_CFG0_TCP_RECV_COPY              1                                 /* TCP���ո����Ż����ڲ�����ʹ�� */
#endif

#ifndef MISCE_CFG0_TCP_OLD_DELETE
#define MISCE_CFG0_TCP_OLD_DELETE             0                                 /* ɾ�������TCP���� */
#endif

/*�����ڴ���� */
#ifndef CH569NET_NUM_IPRAW
#define CH569NET_NUM_IPRAW                    4                                 /* IPRAW���ӵĸ��� */
#endif

#ifndef CH569NET_NUM_UDP
#define CH569NET_NUM_UDP                      4                                 /* UDP���ӵĸ��� */
#endif

#ifndef CH569NET_NUM_TCP
#define CH569NET_NUM_TCP                      4                                 /* TCP���ӵĸ��� */
#endif

#ifndef CH569NET_NUM_TCP_LISTEN
#define CH569NET_NUM_TCP_LISTEN               4                                 /* TCP�����ĸ��� */
#endif

#ifndef CH569NET_NUM_PBUF
#define CH569NET_NUM_PBUF                     16                                 /* PBUF�ṹ�ĸ��� */
#endif

#ifndef CH569NET_NUM_POOL_BUF
#define CH569NET_NUM_POOL_BUF                 0                                 /* POOL BUF�ĸ��� */
#endif

#ifndef CH569NET_NUM_TCP_SEG
#define CH569NET_NUM_TCP_SEG                  16                                 /* tcp�εĸ���*/
#endif

#ifndef CH569NET_NUM_IP_REASSDATA
#define CH569NET_NUM_IP_REASSDATA             16                                 /* IP�ֶεĳ��� */
#endif

/*�ڴ�ع����ڴ��*/
#ifndef CH569NET_NUM_POOL_256
#define CH569NET_NUM_POOL_256                 12                               /* 256�ֽڵĻ��������� */
#endif

#ifndef CH569NET_NUM_POOL_512
#define CH569NET_NUM_POOL_512                 4                                 /* 512�ֽڵĻ��������� */
#endif

#ifndef CH569NET_NUM_POOL_1024
#define CH569NET_NUM_POOL_1024                0                                 /* 1024�ֽڵĻ��������� */
#endif

#ifndef CH569NET_NUM_POOL_1512
#define CH569NET_NUM_POOL_1512                0                                 /* 1512�ֽڵĻ��������� */
#endif
/* end add by ltp */


#ifndef CH569NET_TCP_MSS
#define CH569NET_TCP_MSS                      1460                              /* tcp MSS�Ĵ�С*/
#endif

#ifndef RECE_BUF_LEN
#define RECE_BUF_LEN                          (1460*1)                           /* ���ջ������Ĵ�С */
#endif

#ifndef CH569_MEM_HEAP_SIZE
#define CH569_MEM_HEAP_SIZE                   4600                             /* �ڴ�Ѵ�С */
#endif

#ifndef CH569NET_NUM_ARP_TABLE
#define CH569NET_NUM_ARP_TABLE                16                                /* ARP�б���� */
#endif

#ifndef CH569NET_MEM_ALIGNMENT
#define CH569NET_MEM_ALIGNMENT                4                                 /* 4�ֽڶ��� */
#endif

#ifndef CH569NET_IP_REASS_PBUFS
#if (CH569NET_NUM_POOL_BUF < 32)
#define CH569NET_IP_REASS_PBUFS               (CH569NET_NUM_POOL_BUF - 1)       /* IP��Ƭ��PBUF���������Ϊ31 */
#else
#define CH569NET_IP_REASS_PBUFS               31
#endif
#endif

#define CH569NET_MISC_CONFIG0                 ((MISCE_CFG0_TCP_SEND_COPY << 0) |\
                                               (MISCE_CFG0_TCP_RECV_COPY << 1) |\
                                               (MISCE_CFG0_TCP_OLD_DELETE << 2)|\
                                               (CH569NET_IP_REASS_PBUFS)<<3)

#define  MemNum_content  CH569NET_NUM_IPRAW,\
                         CH569NET_NUM_UDP,\
                         CH569NET_NUM_TCP,\
                         CH569NET_NUM_TCP_LISTEN,\
                         CH569NET_NUM_TCP_SEG,\
                         CH569NET_NUM_IP_REASSDATA,\
                         CH569NET_NUM_PBUF,\
                         CH569NET_NUM_POOL_BUF,\
                         CH569NET_NUM_POOL_256,\
                         CH569NET_NUM_POOL_512,\
                         CH569NET_NUM_POOL_1024,\
                         CH569NET_NUM_POOL_1512

#define MemSize_content  CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_IPRAW_PCB),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_UDP_PCB),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_TCP_PCB),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_TCP_PCB_LISTEN),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_TCP_SEG),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_IP_REASSDATA),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_PBUF) + CH569NET_MEM_ALIGN_SIZE(0),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_PBUF) + CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_BUF),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_256),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_512),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_1024),\
                         CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_1512)



#ifndef CH569NET_MEM_ALIGNMENT
#define CH569NET_MEM_ALIGNMENT                4                                 /* 4�ֽڶ��� */
#endif

#define CH569NET_MEM_ALIGN_SIZE(size)         (((size) + CH569NET_MEM_ALIGNMENT - 1) & ~(CH569NET_MEM_ALIGNMENT-1))/* ������Ա�֤sizeһ���ܱ�16���� */
#define CH569NET_SIZE_IPRAW_PCB               0x1C                              /* IPRAW PCB��С */
#define CH569NET_SIZE_UDP_PCB                 0x20                              /* UDP PCB��С */
#define CH569NET_SIZE_TCP_PCB                 0xAC                              /* TCP PCB��С */
#define CH569NET_SIZE_TCP_PCB_LISTEN          0x20                              /* TCP LISTEN PCB��С */
#define CH569NET_SIZE_IP_REASSDATA            0x20                              /* IP��Ƭ����  */
#define CH569NET_SIZE_PBUF                    0x10                              /* Packet Buf */
#define CH569NET_SIZE_TCP_SEG                 0x14                              /* TCP SEG�ṹ */
#define CH569NET_SIZE_MEM                     0x06                              /* sizeof(struct mem) */
#define CH569NET_SIZE_ARP_TABLE               0x10                              /* sizeof arp table */

/* �����ڴ�ع����ڴ�Ѳ��� */
#define CH569NET_SIZE_POOL_256                0x100
#define CH569NET_SIZE_POOL_512                0x200
#define CH569NET_SIZE_POOL_1024               0x400
#define CH569NET_SIZE_POOL_1512               0x600
/* end add by ltp */


#define CH569NET_SIZE_POOL_BUF                CH569NET_MEM_ALIGN_SIZE(CH569NET_TCP_MSS + 40 +14) /* pbuf�ش�С */
#define CH569NET_MEMP_SIZE                    ((CH569NET_MEM_ALIGNMENT - 1) + \
                                              (CH569NET_NUM_IPRAW * CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_IPRAW_PCB)) + \
                                              (CH569NET_NUM_UDP * CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_UDP_PCB)) + \
                                              (CH569NET_NUM_TCP * CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_TCP_PCB)) + \
                                              (CH569NET_NUM_TCP_LISTEN * CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_TCP_PCB_LISTEN)) + \
                                              (CH569NET_NUM_TCP_SEG * CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_TCP_SEG)) + \
                                              (CH569NET_NUM_IP_REASSDATA * CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_IP_REASSDATA)) + \
                                              (CH569NET_NUM_PBUF * (CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_PBUF) + CH569NET_MEM_ALIGN_SIZE(0))) + \
                                              (CH569NET_NUM_POOL_BUF * (CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_PBUF) + CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_BUF)))) + \
                                              (CH569NET_NUM_POOL_256 *  CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_256)) + \
                                              (CH569NET_NUM_POOL_512 *  CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_512)) + \
                                              (CH569NET_NUM_POOL_1024 *  CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_1024)) + \
                                              (CH569NET_NUM_POOL_1512 *  CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_POOL_1512))


#define  HEAP_MEM_ALIGN_SIZE                  (CH569NET_MEM_ALIGN_SIZE(CH569NET_SIZE_MEM))
#define  CH569NET_RAM_HEAP_SIZE               (CH569_MEM_HEAP_SIZE + (2 * HEAP_MEM_ALIGN_SIZE) + CH569NET_MEM_ALIGNMENT)
#define  CH569NET_RAM_ARP_TABLE_SIZE          (CH569NET_SIZE_ARP_TABLE * CH569NET_NUM_ARP_TABLE)

/* Socket ����ģʽ����,Э������ */
#define PROTO_TYPE_IP_RAW                     0                                 /* IP��ԭʼ���� */
#define PROTO_TYPE_UDP                        2                                 /* UDPЭ������ */
#define PROTO_TYPE_TCP                        3                                 /* TCPЭ������ */

/* �ж�״̬ */
/* ����ΪGLOB_INT�������״̬ */
#define GINT_STAT_UNREACH                     (1<<0)                            /* ���ɴ��ж�*/
#define GINT_STAT_IP_CONFLI                   (1<<1)                            /* IP��ͻ*/
//#define GINT_STAT_PHY_CHANGE                  (1<<2)                            /* PHY״̬�ı� */
#define GINT_STAT_SOCKET                      (1<<4)                            /* scoket �����ж� */

/*����ΪSn_INT�������״̬*/
#define SINT_STAT_RECV                        (1<<2)                            /* socket�˿ڽ��յ����ݻ��߽��ջ�������Ϊ�� */
#define SINT_STAT_CONNECT                     (1<<3)                            /* ���ӳɹ�,TCPģʽ�²������ж� */
#define SINT_STAT_DISCONNECT                  (1<<4)                            /* ���ӶϿ�,TCPģʽ�²������ж� */
#define SINT_STAT_TIM_OUT                     (1<<6)                            /* ARP��TCPģʽ�»ᷢ�����ж� */

/* ������ */
#define CH569NET_ERR_SUCCESS                  0x00                              /* ��������ɹ� */
#define CH569NET_RET_ABORT                    0x5F                              /* �������ʧ�� */
#define CH569NET_ERR_BUSY                     0x10                              /* æ״̬����ʾ��ǰ����ִ������ */
#define CH569NET_ERR_MEM                      0x11                              /* �ڴ���� */
#define CH569NET_ERR_BUF                      0x12                              /* ���������� */
#define CH569NET_ERR_TIMEOUT                  0x13                              /* ��ʱ */
#define CH569NET_ERR_RTE                      0x14                              /* ·�ɴ���*/
#define CH569NET_ERR_ABRT                     0x15                              /* ����ֹͣ*/
#define CH569NET_ERR_RST                      0x16                              /* ���Ӹ�λ */
#define CH569NET_ERR_CLSD                     0x17                              /* ���ӹر�/socket �ڹر�״̬*/
#define CH569NET_ERR_CONN                     0x18                              /* ������ */
#define CH569NET_ERR_VAL                      0x19                              /* �����ֵ */
#define CH569NET_ERR_ARG                      0x1a                              /* ����Ĳ��� */
#define CH569NET_ERR_USE                      0x1b                              /* �Ѿ���ʹ�� */
#define CH569NET_ERR_IF                       0x1c                              /* MAC����  */
#define CH569NET_ERR_ISCONN                   0x1d                              /* ������ */
#define CH569NET_ERR_SOCKET_MEM               0X20                              /* Socket��Ϣ�б��������ߴ��� */
#define CH569NET_ERR_UNSUPPORT_PROTO          0X21                              /* ��֧�ֵ�Э������ */
#define CH569NET_ERR_UNKNOW                   0xFA                              /* δ֪���� */

/* ���ɴ���� */
#define UNREACH_CODE_HOST                     0                                 /* �������ɴ� */
#define UNREACH_CODE_NET                      1                                 /* ���粻�ɴ� */
#define UNREACH_CODE_PROTOCOL                 2                                 /* Э�鲻�ɴ� */
#define UNREACH_CODE_PROT                     3                                 /* �˿ڲ��ɴ� */
/*����ֵ��ο�RFC792�ĵ�*/

/* TCP�رղ��� */
#define TCP_CLOSE_NORMAL                      0                                 /* �����رգ�����4������ */
#define TCP_CLOSE_RST                         1                                 /* ��λ���ӣ����ر�  */
#define TCP_CLOSE_ABANDON                     2                                 /* CH569NET�ڲ��������ӣ����ᷢ���κ���ֹ���� */

/* socket״̬ */
#define  SOCK_STAT_CLOSED                     0X00                              /* socket�ر� */
#define  SOCK_STAT_OPEN                       0X05                              /* socket�� */

/* TCP״̬ */
#define TCP_CLOSED                            0                                 /* TCP���� */
#define TCP_LISTEN                            1                                 /* TCP�ر� */
#define TCP_SYN_SENT                          2                                 /* SYN���ͣ��������� */
#define TCP_SYN_RCVD                          3                                 /* SYN���գ����յ��������� */
#define TCP_ESTABLISHED                       4                                 /* TCP���ӽ��� */
#define TCP_FIN_WAIT_1                        5                                 /* WAIT_1״̬ */
#define TCP_FIN_WAIT_2                        6                                 /* WAIT_2״̬ */
#define TCP_CLOSE_WAIT                        7                                 /* �ȴ��ر� */
#define TCP_CLOSING                           8                                 /* ���ڹر� */
#define TCP_LAST_ACK                          9                                 /* LAST_ACK*/
#define TCP_TIME_WAIT                         10                                /* 2MSL�ȴ� */

#define mStopIfError(x)\
do{if(x!=0){printf("Error: %02x,@ line %d of \"%s\".\r\n", (uint16_t)x,__LINE__,__FILE__);while(1);}}while(0)


/* sokcet��Ϣ�� */
typedef struct _SCOK_INF
{
    UINT32 IntStatus;                                                           /* �ж�״̬ */
    UINT32 SockIndex;                                                           /* Socket����ֵ */
    UINT32 RecvStartPoint;                                                      /* ���ջ������Ŀ�ʼָ�� */
    UINT32 RecvBufLen;                                                          /* ���ջ��������� */
    UINT32 RecvCurPoint;                                                        /* ���ջ������ĵ�ǰָ�� */
    UINT32 RecvReadPoint;                                                       /* ���ջ������Ķ�ָ�� */
    UINT32 RecvRemLen;                                                          /* ���ջ�������ʣ�೤�� */
    UINT32 ProtoType;                                                           /* Э������ */
    UINT32 ScokStatus;                                                          /* ���ֽ�Socket״̬���ε��ֽ�ΪTCP״̬����TCPģʽ�������� */
    UINT32 DesPort;                                                             /* Ŀ�Ķ˿� */
    UINT32 SourPort;                                                            /* Դ�˿���IPRAWģʽ��ΪЭ������ */
    UINT8  IPAddr[4];                                                           /* SocketĿ��IP��ַ 32bit*/
    void *Resv1;                                                                /* �������ڲ�ʹ�� */
    void *Resv2;                                                                /* �������ڲ�ʹ�ã�TCP Serverʹ�� */
 // void (*RecvCallBack)(struct _SCOK_INF *socinf,UINT32 ipaddr,UINT16 port,UINT8 *buf,UINT32 len); /* ���ջص�����*/
    void (*AppCallBack)(struct _SCOK_INF *,UINT32 ,UINT16 ,UINT8 *,UINT32 ); /* ���ջص�����*/

}SOCK_INF;

/* CH569ȫ����Ϣ */
struct _CH569_SYS
{
    UINT8  IPAddr[4];                                                           /* CH569IP��ַ 32bit */
    UINT8  GWIPAddr[4];                                                         /* CH569���ص�ַ 32bit */
    UINT8  MASKAddr[4];                                                         /* CH569�������� 32bit */
    UINT8  MacAddr[8];                                                          /* CH569MAC��ַ 48bit */
    UINT8  UnreachIPAddr[4];                                                    /* ���ɵ���IP */
    UINT32 RetranCount;                                                         /* ���Դ��� Ĭ��Ϊ10�� */
    UINT32 RetranPeriod;                                                        /* ��������,��λMS,Ĭ��200MS */
    UINT32 PHYStat;                                                             /* CH569PHY״̬�� 8bit */
    UINT32 CH569Stat;                                                           /* CH569��״̬ �������Ƿ�򿪵� */
    UINT32 MackFilt;                                                            /* CH569 MAC���ˣ�Ĭ��Ϊ���չ㲥�����ձ���MAC 8bit */
    UINT32 GlobIntStatus;                                                       /* ȫ���ж� */
    UINT32 UnreachCode;                                                         /* ���ɴ� */
    UINT32 UnreachProto;                                                        /* ���ɴ�Э�� */
    UINT32 UnreachPort;                                                         /* ���ɵ���˿� */
    UINT32 SendFlag;
};

/* �ڴ��Լ��������� */
struct _CH569_CFG
{
    UINT32 RxBufSize;                                                            /* MAC���ջ�������С */
    UINT32 TCPMss;                                                               /* TCP MSS��С */
    UINT32 HeapSize;                                                             /* �ѷ����ڴ��С */
    UINT32 ARPTableNum;                                                          /* ARP�б���� */
    UINT32 MiscConfig0;                                                          /* ������������ */
};

/* KEEP LIVE���ýṹ�� */
struct _KEEP_CFG
{
   UINT32 KLIdle;                                                               /* KEEPLIVE����ʱ�� */
   UINT32 KLIntvl;                                                              /* KEEPLIVE���� */
   UINT32 KLCount;                                                              /* KEEPLIVE���� */
};

extern UINT8  CH569IPIntStatus;                                    /* �ж�״̬ */
extern struct _CH569_SYS   CH569Inf;
extern UINT32 CH569NETConfig;                                      /* �����ã�����˵�� */
/* λ0-4 Socket�ĸ���,���ֵΪ31 */
/* λ5-8 MAC �����������ĸ��������ֵΪ15 */
/* λ13 PINGʹ�ܣ�1Ϊ����PING��0Ϊ�ر�PING��Ĭ��Ϊ���� */
/* λ14-18 TCP���Դ���*/
/* λ19-23 TCP�������ڣ���λΪ50����*/
/* λ24 ��̫���ж����� */
/* λ25 ������������ */

struct ethernet_if_fn
{
    UINT8(*send_fn)(UINT32 sendlength);
    UINT8* (*get_send_ptr)(void);
    UINT16(*rece_fn)(UINT8**receptr);
};
extern struct ethernet_if_fn ethernet_if;

//#define USE_DHCP                               /* �Ƿ�ʹ��DHCP,��ʹ���벻Ҫ����USE_DHCP��  */

/*------------------------Э��ջ�����ӿ�---------------------------*/
UINT8 CH569NET_Init( UINT8* ip, UINT8* gwip, UINT8* mask, UINT8* macaddr); /* ���ʼ�� */

UINT8 CH569NET_GetVer(void);                                                    /* ��ѯ��İ汾�� */
#if 1
UINT8 CH569NET_ConfigLIB(struct _CH569_CFG *cfg);                               /* ���ÿ�*/
#endif
void  CH569NET_MainTask(void);                                                  /* ��������������Ҫһֱ���ϵ��� */

void CH569NET_TimeIsr(UINT16 timperiod);                                        /* ʱ���жϷ�����������ǰ������ʱ������ */

UINT8 CH569NET_QueryGlobalInt(void);                                             /* ��ѯȫ���ж� */

UINT8 CH569NET_GetGlobalInt (void);                                             /* ��ȫ���жϲ���ȫ���ж����� */

void CH569NET_OpenMac(void);                                                    /* ��MAC */

void CH569NET_CloseMac(void);                                                   /* �ر�MAC */

UINT8 CH569NET_SocketCreat(UINT8 *socketid,SOCK_INF *socinf);                   /* ����socket */

UINT8 CH569NET_SocketSend(UINT8 socketid,UINT8 *buf,UINT32 *len);               /* Socket�������� */

UINT8 CH569NET_SocketRecv(UINT8 socketid,UINT8 *buf,UINT32 *len);               /* Socket�������� */

UINT8 CH569NET_GetSocketInt(UINT8 sockedid);                                    /* ��ȡsocket�жϲ����� */

UINT32 CH569NET_SocketRecvLen(UINT8 socketid,UINT32 *bufaddr);                  /* ��ȡsocket���ճ��� */

UINT8 CH569NET_SocketConnect(UINT8 socketid);                                   /* TCP����*/

UINT8 CH569NET_SocketListen(UINT8 socindex);                                    /* TCP���� */

UINT8 CH569NET_SocketClose(UINT8 socindex,UINT8 flag);                          /* �ر����� */

void CH569NET_ModifyRecvBuf(UINT8 sockeid,UINT32 bufaddr,UINT32 bufsize);       /* �޸Ľ��ջ����� */

UINT8 CH569NET_SocketUdpSendTo(UINT8 socketid, UINT8 *buf, UINT32 *slen,UINT8 *sip,UINT16 port);/* ��ָ����Ŀ��IP���˿ڷ���UDP�� */

UINT8 CH569NET_Aton(const UINT8 *cp, UINT8 *addr);                              /* ASCII���ַת�����ַ */

UINT8 *CH569NET_Ntoa(UINT8 *ipaddr);                                            /* �����ַתASCII��ַ */

UINT8 CH569NET_SetSocketTTL(UINT8 socketid, UINT8 ttl);                         /* ����socket��TTL */

void CH569NET_RetrySendUnack(UINT8 socketid);                                   /* TCP�ش� */

UINT8 CH569NET_QueryUnack(SOCK_INF  *sockinf,UINT32 *addrlist,UINT16 lislen)    /* ��ѯδ���ͳɹ������ݰ� */;

UINT8 CH569NET_DHCPStart(UINT8(* usercall)(UINT8 status,void *));               /* DHCP���� */

UINT8 CH569NET_DHCPStop(void);                                                  /* DHCPֹͣ */

void CH569NET_ConfigKeepLive(struct _KEEP_CFG *cfg);                            /* ���ÿ�KEEP LIVE���� */

UINT8 CH569NET_SocketSetKeepLive(UINT8 socindex,UINT8 cfg);                     /* ����socket KEEP LIVE*/

void frame_input(void);

void reset_net_para(UINT8 *ip,UINT8 *mask,UINT8 *gateway);
#endif

