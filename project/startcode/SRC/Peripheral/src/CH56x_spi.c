/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_spi.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
*******************************************************************************/

#include "CH56x_common.h"

/*******************************************************************************
* Function Name  : SPI0_MasterDefInit
* Description    : ����ģʽĬ�ϳ�ʼ��
* Input          : None
* Return         : None
*******************************************************************************/
void SPI0_MasterDefInit( void )
{
    R8_SPI0_CLOCK_DIV = 4;                                   //��Ƶʱ��4��Ƶ
    R8_SPI0_CTRL_MOD = RB_SPI_ALL_CLEAR;                     //FIFO/������/�жϱ�־�Ĵ�����0��д1ǿ����ջ�����
    R8_SPI0_CTRL_MOD = RB_SPI_MOSI_OE | RB_SPI_SCK_OE ;      //MOSI���ź�SCK�������ʹ��
    R8_SPI0_CTRL_CFG |= RB_SPI_AUTO_IF;                      //ʹ�ܷ���BUFFER/FIFO�Զ������־λ
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;                  //������DMA��ʽ
}

/*******************************************************************************
* Function Name  : SPI0_DataMode
* Description    : ����������ģʽ
* Input          : m: ������ģʽ
					refer to ModeBitOrderTypeDef
* Return         : None
*******************************************************************************/
void SPI0_DataMode( ModeBitOrderTypeDef m )
{
    switch( m )
    {
        case Mode0_LowBitINFront:                       //ģʽ0����λ��ǰ
            R8_SPI0_CTRL_MOD &= ~RB_SPI_MST_SCK_MOD;
            R8_SPI0_CTRL_CFG |= RB_SPI_BIT_ORDER;
            break;
        case Mode0_HighBitINFront:                      //ģʽ0����λ��ǰ
            R8_SPI0_CTRL_MOD &= ~RB_SPI_MST_SCK_MOD;
            R8_SPI0_CTRL_CFG &= ~RB_SPI_BIT_ORDER;
            break;
        case Mode3_LowBitINFront:                       //ģʽ3����λ��ǰ
            R8_SPI0_CTRL_MOD |= RB_SPI_MST_SCK_MOD;
            R8_SPI0_CTRL_CFG |= RB_SPI_BIT_ORDER;
            break;
        case Mode3_HighBitINFront:                      //ģʽ3����λ��ǰ
            R8_SPI0_CTRL_MOD |= RB_SPI_MST_SCK_MOD;
            R8_SPI0_CTRL_CFG &= ~RB_SPI_BIT_ORDER;
            break;
        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : SPI0_MasterSendByte
* Description    : ���͵��ֽ�(buffer)
* Input          : d:�����ֽ�
* Return         : None
*******************************************************************************/
void SPI0_MasterSendByte( UINT8 d )
{
    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI0_BUFFER = d;
    while( !(R8_SPI0_INT_FLAG & RB_SPI_FREE) );
}

/*******************************************************************************
* Function Name  : SPI0_MasterRecvByte
* Description    : ���յ��ֽ� (buffer)
* Input          : None
* Return         : ���յ����ֽ�
*******************************************************************************/
UINT8 SPI0_MasterRecvByte( void )
{
    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI0_BUFFER = 0xFF;                                //��������
    while( !(R8_SPI0_INT_FLAG & RB_SPI_FREE) );
    return ( R8_SPI0_BUFFER );
}

/*******************************************************************************
* Function Name  : SPI0_MasterTrans
* Description    : ʹ��FIFO�������Ͷ��ֽ�
* Input          : pbuf:�����͵����������׵�ַ
                   len: �����͵����ݳ��ȣ����4095
* Return         : None
*******************************************************************************/
void SPI0_MasterTrans( UINT8 *pbuf, UINT16 len )
{
    UINT16 sendlen;

    sendlen = len;
    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;        //�������ݷ���Ϊ���
    R16_SPI0_TOTAL_CNT = sendlen;                //����Ҫ���͵����ݳ���
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END;
    while( sendlen )
    {
        if( R8_SPI0_FIFO_COUNT < SPI_FIFO_SIZE )
        {
            R8_SPI0_FIFO = *pbuf;
            pbuf++;
            sendlen--;
        }
    }
    while( R8_SPI0_FIFO_COUNT != 0 );             //�ȴ�FIFO�е�����ȫ���������
}

/*******************************************************************************
* Function Name  : SPI0_MasterRecv
* Description    : ʹ��FIFO�������ն��ֽ�
* Input          : pbuf: �����͵����������׵�ַ
                   len: �����͵����ݳ��ȣ����4095
* Return         : None
*******************************************************************************/
void SPI0_MasterRecv( UINT8 *pbuf, UINT16 len )
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
* Function Name  : SPI0_MasterDMATrans
* Description    : DMA��ʽ������������
* Input          : pbuf: ������������ʼ��ַ
*                  len : ���������ݳ���
* Return         : None
*******************************************************************************/
void SPI0_MasterDMATrans( PUINT8 pbuf, UINT16 len)
{
    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R32_SPI0_DMA_BEG = (UINT32)pbuf;
    R32_SPI0_DMA_END = (UINT32)(pbuf + len);
    R16_SPI0_TOTAL_CNT = len;
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END|RB_SPI_IF_DMA_END;
    R8_SPI0_CTRL_CFG |= RB_SPI_DMA_ENABLE;
    while(!(R8_SPI0_INT_FLAG & RB_SPI_IF_CNT_END));
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}

/*******************************************************************************
* Function Name  : SPI0_MasterDMARecv
* Description    : DMA��ʽ������������
* Input          : pbuf: ���������ݴ����ʼ��ַ
*                  len : ���������ݳ���
* Return         : None
*******************************************************************************/
void SPI0_MasterDMARecv( PUINT8 pbuf, UINT16 len)
{
    R8_SPI0_CTRL_MOD |= RB_SPI_FIFO_DIR;
    R32_SPI0_DMA_BEG = (UINT32)pbuf;
    R32_SPI0_DMA_END = (UINT32)(pbuf + len);
    R16_SPI0_TOTAL_CNT = len;
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END|RB_SPI_IF_DMA_END;
    R8_SPI0_CTRL_CFG |= RB_SPI_DMA_ENABLE;
    while(!(R8_SPI0_INT_FLAG & RB_SPI_IF_CNT_END));
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}

/*******************************************************************************
* Function Name  : SPI0_SlaveInit
* Description    : �豸ģʽĬ�ϳ�ʼ��
* Input          : None
* Return         : None
*******************************************************************************/
void SPI0_SlaveInit( void )
{
    R8_SPI0_CTRL_MOD = RB_SPI_ALL_CLEAR;             //FIFO/������/�жϱ�־�Ĵ�����0��д1ǿ����ջ�����
    R8_SPI0_CTRL_MOD = RB_SPI_MISO_OE | RB_SPI_MODE_SLAVE;
    R8_SPI0_CTRL_CFG |= RB_SPI_AUTO_IF;              //ʹ�ܷ���BUFFER/FIFO�Զ������־λ
}

/*******************************************************************************
* Function Name  : SPI0_SlaveRecvByte
* Description    : �ӻ�ģʽ������һ�ֽ�����
* Input          : None
* Return         : ���յ�����
*******************************************************************************/
UINT8 SPI0_SlaveRecvByte( void )
{
    R8_SPI0_CTRL_MOD |= RB_SPI_FIFO_DIR;              //����Ϊ����ģʽ����������
    while( R8_SPI0_FIFO_COUNT == 0 );
    return R8_SPI0_FIFO;
}

/*******************************************************************************
* Function Name  : SPI0_SlaveRecvByte
* Description    : �ӻ�ģʽ������һ�ֽ�����
* Input          : None
* Return         : ���յ�����
*******************************************************************************/
void SPI0_SlaveSendByte( UINT8 d )
{
    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;              //�������ݷ���Ϊ���
    R8_SPI0_FIFO = d;
    while( R8_SPI0_FIFO_COUNT != 0 );                  //�ȴ��������
}

/*******************************************************************************
* Function Name  : SPI0_SlaveRecv
* Description    : �ӻ�ģʽ�����ն��ֽ�����
* Input          : pbuf: �������ݴ����ʼ��ַ
*                  len : ����������ݳ���
* Return         : None
*******************************************************************************/
void SPI0_SlaveRecv( PUINT8 pbuf, UINT16 len )
{
    UINT16 revlen;

    revlen = len;
    R8_SPI0_CTRL_MOD |= RB_SPI_FIFO_DIR;                //����Ϊ����ģʽ����������
    R16_SPI0_TOTAL_CNT = revlen;                        //��SPI�շ������ܳ��ȼĴ������и�ֵ
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END;               //SPI�жϱ�־�Ĵ���  ȫ���ֽڴ�����ɱ�־��д1��0
    while( revlen )
    {
        if( R8_SPI0_FIFO_COUNT )                        //��ǰFIFO���ֽڼ���
        {
            *pbuf = R8_SPI0_FIFO;
            pbuf++;
            revlen--;
        }
    }
}

/*******************************************************************************
* Function Name  : SPI0_SlaveTrans
* Description    : �ӻ�ģʽ�����Ͷ��ֽ�����
* Input          : pbuf: �����͵����������׵�ַ
                   len: �����͵����ݳ��ȣ����4095
* Return         : None
*******************************************************************************/
void SPI0_SlaveTrans( UINT8 *pbuf, UINT16 len )
{
    UINT16 sendlen;

    sendlen = len;
    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;              //�������ݷ���Ϊ���
    R16_SPI0_TOTAL_CNT = sendlen;                      //����Ҫ���͵����ݳ���
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END;              //SPI�жϱ�־�Ĵ���  ȫ���ֽڴ�����ɱ�־��д1��0
    while( sendlen )
    {
        if( R8_SPI0_FIFO_COUNT < SPI_FIFO_SIZE )       //�Ƚϵ�ǰFIFO���ֽڼ�����С
        {
            R8_SPI0_FIFO = *pbuf;
            pbuf++;
            sendlen--;
        }
    }
    while( R8_SPI0_FIFO_COUNT != 0 );                  //�ȴ�FIFO�е�����ȫ���������
}


/*******************************************************************************
* Function Name  : SPI1_MasterDefInit
* Description    : ����ģʽĬ�ϳ�ʼ��
* Input          : None
* Return         : None
*******************************************************************************/
void SPI1_MasterDefInit( void )
{
    R8_SPI1_CLOCK_DIV = 4;                                   //��Ƶʱ��4��Ƶ
    R8_SPI1_CTRL_MOD = RB_SPI_ALL_CLEAR;
    R8_SPI1_CTRL_MOD = RB_SPI_MOSI_OE | RB_SPI_SCK_OE ;
    R8_SPI1_CTRL_CFG |= RB_SPI_AUTO_IF;
    R8_SPI1_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;                  //������DMA��ʽ
    //R8_SPI1_CTRL_CFG |= RB_SPI_DMA_ENABLE;                   //����DMA��ʽ
}

/*******************************************************************************
* Function Name  : SPI1_DataMode
* Description    : ����������ģʽ
* Input          : m: ������ģʽ
					refer to ModeBitOrderTypeDef
* Return         : None
*******************************************************************************/
void SPI1_DataMode( ModeBitOrderTypeDef m )
{
    switch( m )
    {
        case Mode0_LowBitINFront:
            R8_SPI1_CTRL_MOD &= ~RB_SPI_MST_SCK_MOD;
            R8_SPI1_CTRL_CFG |= RB_SPI_BIT_ORDER;
            break;
        case Mode0_HighBitINFront:
            R8_SPI1_CTRL_MOD &= ~RB_SPI_MST_SCK_MOD;
            R8_SPI1_CTRL_CFG &= ~RB_SPI_BIT_ORDER;
            break;
        case Mode3_LowBitINFront:
            R8_SPI1_CTRL_MOD |= RB_SPI_MST_SCK_MOD;
            R8_SPI1_CTRL_CFG |= RB_SPI_BIT_ORDER;
            break;
        case Mode3_HighBitINFront:
            R8_SPI1_CTRL_MOD |= RB_SPI_MST_SCK_MOD;
            R8_SPI1_CTRL_CFG &= ~RB_SPI_BIT_ORDER;
            break;
        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : SPI1_MasterSendByte
* Description    : ���͵��ֽ�(buffer)
* Input          : d:�����ֽ�
* Return         : None
*******************************************************************************/
void SPI1_MasterSendByte( UINT8 d )
{
    R8_SPI1_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI1_BUFFER = d;
    while( !(R8_SPI1_INT_FLAG & RB_SPI_FREE) );
}

/*******************************************************************************
* Function Name  : SPI1_MasterRecvByte
* Description    : ���յ��ֽ� (buffer)
* Input          : None
* Return         : ���յ����ֽ�
*******************************************************************************/
UINT8 SPI1_MasterRecvByte( void )
{
    R8_SPI1_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI1_BUFFER = 0xFF;                     //��������
    while( !(R8_SPI1_INT_FLAG & RB_SPI_FREE) );
    return ( R8_SPI1_BUFFER );
}

/*******************************************************************************
* Function Name  : SPI1_MasterTrans
* Description    : ʹ��FIFO�������Ͷ��ֽ�
* Input          : pbuf:�����͵����������׵�ַ
                   len: �����͵����ݳ��ȣ����4095
* Return         : None
*******************************************************************************/
void SPI1_MasterTrans( UINT8 *pbuf, UINT16 len )
{
    UINT16 sendlen;

    sendlen = len;
    R8_SPI1_CTRL_MOD &= ~RB_SPI_FIFO_DIR;        //�������ݷ���Ϊ���
    R16_SPI1_TOTAL_CNT = sendlen;                //����Ҫ���͵����ݳ���
    R8_SPI1_INT_FLAG = RB_SPI_IF_CNT_END;
    while( sendlen )
    {
        if( R8_SPI1_FIFO_COUNT < SPI_FIFO_SIZE )
        {
            R8_SPI1_FIFO = *pbuf;
            pbuf++;
            sendlen--;
        }
    }
    while( R8_SPI1_FIFO_COUNT != 0 );             //�ȴ�FIFO�е�����ȫ���������
}

/*******************************************************************************
* Function Name  : SPI1_MasterRecv
* Description    : ʹ��FIFO�������ն��ֽ�
* Input          : pbuf: �����͵����������׵�ַ
                   len: �����͵����ݳ��ȣ����4095
* Return         : None
*******************************************************************************/
void SPI1_MasterRecv( UINT8 *pbuf, UINT16 len )
{
    UINT16  readlen;

    readlen = len;
    R8_SPI1_CTRL_MOD |= RB_SPI_FIFO_DIR;         //�������ݷ���Ϊ����
    R16_SPI1_TOTAL_CNT = len;                    //������Ҫ���յ����ݳ��ȣ�FIFO����Ϊ���볤�Ȳ�Ϊ0�����������
    R8_SPI1_INT_FLAG = RB_SPI_IF_CNT_END;
    while( readlen )
    {
        if( R8_SPI1_FIFO_COUNT )
        {
            *pbuf = R8_SPI1_FIFO;
            pbuf++;
            readlen--;
        }
    }
}

/*******************************************************************************
* Function Name  : SPI1_MasterDMATrans
* Description    : DMA��ʽ������������
* Input          : pbuf: ������������ʼ��ַ
*                  len : ���������ݳ���
* Return         : None
*******************************************************************************/
void SPI1_MasterDMATrans( PUINT8 pbuf, UINT16 len)
{
    R8_SPI1_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R32_SPI1_DMA_BEG = (UINT32)pbuf;
    R32_SPI1_DMA_END = (UINT32)(pbuf + len);
    R16_SPI1_TOTAL_CNT = len;
    R8_SPI1_INT_FLAG = RB_SPI_IF_CNT_END|RB_SPI_IF_DMA_END;
    R8_SPI1_CTRL_CFG |= RB_SPI_DMA_ENABLE;
    while(!(R8_SPI1_INT_FLAG & RB_SPI_IF_CNT_END));
    R8_SPI1_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}

/*******************************************************************************
* Function Name  : SPI1_MasterDMARecv
* Description    : DMA��ʽ������������
* Input          : pbuf: ���������ݴ����ʼ��ַ
*                  len : ���������ݳ���
* Return         : None
*******************************************************************************/
void SPI1_MasterDMARecv( PUINT8 pbuf, UINT16 len)
{
    R8_SPI1_CTRL_MOD |= RB_SPI_FIFO_DIR;
    R32_SPI1_DMA_BEG = (UINT32)pbuf;
    R32_SPI1_DMA_END = (UINT32)(pbuf + len);
    R16_SPI1_TOTAL_CNT = len;
    R8_SPI1_INT_FLAG = RB_SPI_IF_CNT_END|RB_SPI_IF_DMA_END;
    R8_SPI1_CTRL_CFG |= RB_SPI_DMA_ENABLE;
    while(!(R8_SPI0_INT_FLAG & RB_SPI_IF_CNT_END));
    R8_SPI1_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}

/*******************************************************************************
* Function Name  : SPI1_SlaveInit
* Description    : �豸ģʽĬ�ϳ�ʼ��
* Input          : None
* Return         : None
*******************************************************************************/
void SPI1_SlaveInit( void )
{
    R8_SPI1_CTRL_MOD = RB_SPI_ALL_CLEAR;
    R8_SPI1_CTRL_MOD = RB_SPI_MISO_OE | RB_SPI_MODE_SLAVE;
    R8_SPI1_CTRL_CFG |= RB_SPI_AUTO_IF;
}

/*******************************************************************************
* Function Name  : SPI1_SlaveRecvByte
* Description    : �ӻ�ģʽ������һ�ֽ�����
* Input          : None
* Return         : ���յ�����
*******************************************************************************/
UINT8 SPI1_SlaveRecvByte( void )
{
    R8_SPI1_CTRL_MOD |= RB_SPI_FIFO_DIR;
    while( R8_SPI1_FIFO_COUNT == 0 );
    return R8_SPI1_FIFO;
}

/*******************************************************************************
* Function Name  : SPI1_SlaveRecvByte
* Description    : �ӻ�ģʽ������һ�ֽ�����
* Input          : None
* Return         : ���յ�����
*******************************************************************************/
void SPI1_SlaveSendByte( UINT8 d )
{
    R8_SPI1_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI1_FIFO = d;
    while( R8_SPI1_FIFO_COUNT != 0 );        //�ȴ��������
}

/*******************************************************************************
* Function Name  : SPI1_SlaveRecv
* Description    : �ӻ�ģʽ�����ն��ֽ�����
* Input          : pbuf: �������ݴ����ʼ��ַ
*                  len : ����������ݳ���
* Return         : None
*******************************************************************************/
void SPI1_SlaveRecv( PUINT8 pbuf, UINT16 len )
{
    UINT16 revlen;

    revlen = len;
    R8_SPI1_CTRL_MOD |= RB_SPI_FIFO_DIR;
    R16_SPI1_TOTAL_CNT = revlen;
    R8_SPI1_INT_FLAG = RB_SPI_IF_CNT_END;
    while( revlen )
    {
        if( R8_SPI1_FIFO_COUNT )
        {
            *pbuf = R8_SPI1_FIFO;
            pbuf++;
            revlen--;
        }
    }
}

/*******************************************************************************
* Function Name  : SPI1_SlaveTrans
* Description    : �ӻ�ģʽ�����Ͷ��ֽ�����
* Input          : pbuf: �����͵����������׵�ַ
                   len: �����͵����ݳ��ȣ����4095
* Return         : None
*******************************************************************************/
void SPI1_SlaveTrans( UINT8 *pbuf, UINT16 len )
{
    UINT16 sendlen;

    sendlen = len;
    R8_SPI1_CTRL_MOD &= ~RB_SPI_FIFO_DIR;              //�������ݷ���Ϊ���
    R16_SPI1_TOTAL_CNT = sendlen;                      //����Ҫ���͵����ݳ���
    R8_SPI1_INT_FLAG = RB_SPI_IF_CNT_END;
    while( sendlen )
    {
        if( R8_SPI1_FIFO_COUNT < SPI_FIFO_SIZE )
        {
            R8_SPI1_FIFO = *pbuf;
            pbuf++;
            sendlen--;
        }
    }
    while( R8_SPI1_FIFO_COUNT != 0 );                   //�ȴ�FIFO�е�����ȫ���������
}







