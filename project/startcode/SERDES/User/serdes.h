
#ifndef __SERDES_H__
#define __SERDES_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "CH56xSFR.h"
#include "core_riscv.h"


#define SDS_ALIGN_EN	(1<<18)
#define SDS_CONT_EN		(1<<17)
#define SDS_POWR_UP		(1<<16)
#define SDS_TX_PU		(1<<15)
#define SDS_RX_PU		(1<<14)
#define SDS_PLL_PU		(1<<13)
#define	SDS_PLL_DIV		(18<<8) // 60*(DIV+2)MHz
#define	SDS_PLL_1_2G	(18<<8) // 60*(18+2)MHz
#define	SDS_PLL_900M	(13<<8) // 60*(13+2)MHz
#define	SDS_PLL_600M	( 8<<8) // 60*(8+2)MHz

#define SDS_DMA_EN              1<<7
#define SDS_TX_EN		1<<6
#define SDS_RX_EN		1<<5
#define SDS_RX_POLAR 	1<<4
#define SDS_INT_BUSY_EN 1<<3
#define PHY_RESET 		1<<2
#define LINK_RESET 		1<<1
#define SDS_ALL_CLR 	1<<0

#define SDS_LINK_INIT	1<<16
#define SDS_TX_START	((uint32_t)1<<17)
#define SDS_BUF_MODE	1<<18

#define SDS_PHY_RDY_EN    1<<0
#define SDS_TX_INT_EN     1<<1
#define SDS_RX_ERR_EN     1<<1
#define SDS_RX_INT_EN     1<<2
#define SDS_FIFO_OV_EN    1<<3
#define SDS_COMMA_INT_EN  1<<5
#define ALL_INT_TYPE      47

#define SDS_PHY_RDY_FLG 	1<<0
#define SDS_TX_INT_FLG		1<<1
#define	SDS_RX_INT_FLG		1<<2
#define	SDS_FIFO_OV_FLG		1<<3
#define	SDS_COMMA_INT_FLG	1<<5
#define ALL_INT_FLG         47

#define SDS_SEQ_MATCH	1<<17
#define SDS_RX_CRC_OK	1<<18
#define SDS_PLL_READY	1<<19
#define SDS_TX_READY	1<<20

#define SDS_RX_BUF		1<<24
#define SDS_TX_BUF		1<<28

#define SDS_TX_NUMP_1	1<<20
#define SDS_TX_NUMP_2	2<<20
#define SDS_TX_NUMP_3	3<<20
#define SDS_TX_NUMP_4	4<<20

/*******************************************************************************
* Function Name  : serdes_Tx_init
* Description    : transfer module initialization
* Input          : None
* Return         : None
*******************************************************************************/
extern void serdes_Tx_init(void);

/*******************************************************************************
* Function Name  : Tx_config
* Description    : transfer DMA configuration
* Input          : DMAaddr--Tx first address
* Return         : None
*******************************************************************************/
extern void Tx_config( UINT32 DMAaddr, UINT32 Tx_len, UINT32 comma);

/*******************************************************************************
* Function Name  : Rx_config
* Description    : receiver DMA configuration
* Input          : DMA0_addr,DMA1_addr--double DMA first address
* Return         : None
*******************************************************************************/
extern void Rx_config( UINT32 DMA0_addr , UINT32 DMA1_addr );

/*******************************************************************************
* Function Name  : DMA_Tx
* Description    : data transfer DMA
* Input          : None
* Return         : None
*******************************************************************************/
extern void DMA_Tx(void);

/*******************************************************************************
* Function Name  : Rx_check
* Description    : check received data
* Input          : DMA0_addr,DMA1_addr--double DMA first address
* Return         : None
*******************************************************************************/
extern void Rx_check( UINT32 DMA0_addr, UINT32 DMA1_addr);

/*******************************************************************************
* Function Name  : ClearITFlag
* Description    : Clear interruption flag
* Input          : ITFlag:
                      1.SDS_PHY_RDY_FLG
                      2.SDS_TX_INT_FLG
                      3.SDS_RX_INT_FLG
                      4.SDS_FIFO_OV_FLG
                      5.SDS_COMMA_INT_FLG
                      6.ALL_INT_FLG
* Return         : None
*******************************************************************************/
extern void ClearITFlag( UINT16 ITFlag);

/*******************************************************************************
* Function Name  : Enable Interrupt
* Description    : ?????
* Input          : ITType:
                      1.SDS_PHY_RDY_EN
                      2(1).SDS_TX_INT_EN      (Tx mode)
                      2(2).SDS_RX_ERR_EN      (Rx mode) 
                      3.SDS_RX_INT_EN
                      4.SDS_FIFO_OV_EN
                      5.SDS_COMMA_INT_EN
                      6.ALL_INT_TYPE
* Return         : None
*******************************************************************************/
extern void EnableIT( UINT16 ITType);

/*******************************************************************************
* Function Name  : wait_Txdone
* Description    : waiting for Tx done
* Input          : None
* Return         : None
*******************************************************************************/
extern void wait_Txdone(void);

/*******************************************************************************
* Function Name  : wait_commadone
* Description    : waiting for Rx COMMA done
* Input          : None
* Return         : None
*******************************************************************************/
extern void wait_commadone(void);

/*******************************************************************************
* Function Name  : ReadRxFlagBit
* Description    : read Rx IT status
* Input          : None
* Return         : None
*******************************************************************************/
extern UINT32 ReadITFlagBit(UINT16 ITFlag);

/*******************************************************************************
* Function Name  : ReadCOMMAFlagBit
* Description    : read COMMA IT status
* Input          : None
* Return         : None
*******************************************************************************/
extern UINT32 ReadCOMMAFlagBit(void);

/*******************************************************************************
* Function Name  : serdes_Rx_init
* Description    : receiver module initialization
* Input          : None
* Return         : None
*******************************************************************************/
extern void serdes_Rx_init(void);


#ifdef __cplusplus
}
#endif

#endif
