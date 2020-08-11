/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : USB3.0 �� USB2.0��������
*******************************************************************************/

#include "CH56xSFR.h"
#include "core_riscv.h"
#include "string.h"
#include "stdio.h"
#include "usbss.h"
#include "CH56X_ENMU.H"
#include "CHRV3UFI.h"
#include "type.h"
#include "debug.h"

#define	FREQ_SYS	80000000
#define	STDIO_BAUD_RATE	115200				//baudrate 1152000BPS
#define NO_DEFAULT_ACCESS_SECTOR			1

__attribute__ ((aligned(4))) UINT8  RxBuffer[1] ;      // IN, must even address
__attribute__ ((aligned(4))) UINT8  TxBuffer[1] ;      // OUT, must even address

UINT32  DMAaddr = Usb_Tx_DMAaddr;
UINT8 tx_lmp_port = 0;
UINT8 enum_ready = 0;

void USBHS_IRQHandler (void) __attribute__((interrupt()));		//USBHS interrupt service
void usbhs_init (void);

/*******************************************************************************
* Function Name  : DebugInit
* Description    : Initializes the UART3 peripheral.
* Input          : baudrate: UART3 communication baud rate.
* Return         : None
*******************************************************************************/
void DebugInit(UINT32 baudrate)
{
	UINT32 x;
	UINT32 t = FREQ_SYS;
	
	x = 10 * t * 2 / 16 / baudrate;
	x = ( x + 5 ) / 10;
	R8_UART3_DIV = 1;
	R16_UART3_DL = x;
	R8_UART3_FCR = RB_FCR_FIFO_TRIG | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN;
	R8_UART3_LCR = RB_LCR_WORD_SZ;
	R8_UART3_IER = RB_IER_TXD_EN;
	R32_PB_SMT |= bRXD3 | bTXD3;
	R32_PB_DIR |= bTXD3;
}

/*******************************************************************************
* Function Name  : usbhs_init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void usbhs_init()	// USBSS device initial
{
    if( R8_SLP_POWER_CTRL ){
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xa8;
        R8_SLP_POWER_CTRL = 0;      // �ϵ�
        R8_SAFE_ACCESS_SIG = 0;
        mDelayuS( 2 );
    }
    R8_SAFE_ACCESS_SIG = 0x57;
    R8_SAFE_ACCESS_SIG = 0xa8;
    R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_USBHS;   // ��ʱ��
    R8_SAFE_ACCESS_SIG = 0;

    R8_USB_CTRL = RB_USB_CLR_ALL  | RB_USB_RESET_SIE;         //����жϱ�־��FIFO������λ
    R8_USB_INT_EN = 0;
    R8_USB_CTRL = RB_USB_MODE | RB_USB_INT_BUSY | UCST_HS | RB_USB_DMA_EN;
    R8_USB_DEV_AD = 0x00;
    R8_UHOST_CTRL = 0x00;                              //����ģʽ�£���������״̬

    R16_UH_RX_MAX_LEN = 512;                 //���������հ��ĳ���
    R8_UH_RX_CTRL = 0;
    R8_UH_TX_CTRL = 0;
    R8_UH_EP_MOD = RB_UH_RX_EN | RB_UH_TX_EN ;       //�������ͽ��͹��ܣ�������ģʽ
}

void USBSS_IRQHandler (void) __attribute__((interrupt()));		
void LINK_IRQHandler (void) __attribute__((interrupt()));
void TMR0_IRQHandler (void) __attribute__((interrupt()));
void usbssh_init (void);

unsigned long NTFS_BUF[ 128 ];//4K
UINT8 *pNTFS_BUF;
UINT8  U30_Check_Time = 0;		//U30��ⳬʱ

/*******************************************************************************
* Function Name  : TMR0_IRQHandler
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TMR0_IRQHandler( void )
{
	if( R8_TMR0_INT_FLAG &0x01 ){
		R8_TMR0_INT_FLAG = 0x01;

		if( gDeviceConnectstatus == USB_INT_CONNECT_U20 ){
			 U30_Check_Time++;
			 if( U30_Check_Time>= 5 ){			//1S�ĳ�ʱʱ��
				 printf("u20_connect\n");
				 gDeviceConnectstatus = USB_INT_CONNECT;
				 U30_Check_Time = 0;
			 }
		}
	}
}

/*******************************************************************************
* Function Name  : TMR0_TimerInit
* Description    : ��ʱ���ܳ�ʼ��
* Input          : t: ��ʱʱ�䣬���ڵ�ǰϵͳʱ��Tsys, ���ʱ���� 67108864
* Return         : None
*******************************************************************************/
void TMR0_TimerInit( void )
{
	R32_TMR0_CNT_END = 8000000;//80*1000*100;		//100MS
	R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;
	R8_TMR0_CTRL_MOD = RB_TMR_COUNT_EN;
	R8_TMR0_INTER_EN |= 0x01;
	PFIC_EnableIRQ( TMR0_IRQn );					//��ʱ��0�ж�ʹ��
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main( ) 
{
	UINT8 s;
	UINT8 s1;
	UINT32	k;
	UINT32	cc,i,temp32;
	UINT32	data;
	UINT32 *p32_txdma = (UINT32 *)DMAaddr;
	UINT8 *pd,*pd1;
	UINT8 buf[8];
	pNTFS_BUF = NTFS_ADDR;
	k = 0;

	// config system clock frequency
	R8_SAFE_ACCESS_SIG = 0x57; // enable safe access mode
	R8_SAFE_ACCESS_SIG = 0xa8;
	R8_CLK_PLL_DIV = 0x40 | 0x06;				// system clock: SYS_FREQ = SRC_FREQ / R8_CLK_PLL_DIV = 480MHz/6=80MHz
	R8_CLK_CFG_CTRL = 0x80| RB_CLK_SEL_PLL ;	// OSC_FREQ = 30MHz 
												// PLL_FREQ = 300MHz/480MHz
												// select system clock source: SRC_FREQ = RB_CLK_SEL_PLL ? PLL_FREQ : OSC_FREQ
	DebugInit(STDIO_BAUD_RATE);
	printf("This is usb3.0 host program(120MHz) !\n");
	usbhs_init();
	PFIC_EnableIRQ( LINK_IRQn );
	TMR0_TimerInit();
	mDelayuS(2000);
	while(1)
	{
		 mDelayuS(2);
		if( gDeviceConnectstatus == USB_INT_CONNECT )
		{
			if( gDeviceUsbType == USB_U30_SPEED ){
				s = U30HSOT_Enumerate( pNTFS_BUF );
			}
			else{
				s = U20HSOT_Enumerate( pNTFS_BUF );
			}
			printf("Enumerate_status=%02x\n",s);
			s = MS_Init(pNTFS_BUF);
			printf("udisk_init=%02x\n",s);
			s = Fat_Init();
			printf("end\n");
			printf("wait_disconnect\n");
			while(gDeviceConnectstatus == USB_INT_CONNECT){		//�ȴ��豸�Ͽ�
				mDelaymS( 100 );

				if( gDeviceUsbType == USB_U20_SPEED ){
					R8_USB_INT_FG = RB_USB_IF_DETECT;
					if( R8_USB_MIS_ST&RB_USB_ATTACH )
					{

					}
					else
					{
						break;
					}
				}
			}
			gDeviceConnectstatus = 0;
			gDeviceUsbType = 0;
			usbssh_init_reset();
			usbhs_init();
			printf("disconnect\n");
		}
		else if(R8_USB_INT_FG&RB_USB_IF_DETECT)		//�Ƚ���USB2.0�豸���ӣ�֮���ڽ���USB3.0�豸���ӳ�ʼ��
		{
			R8_USB_INT_FG = RB_USB_IF_DETECT;
			if( R8_USB_MIS_ST&RB_USB_ATTACH )
			{
				printf("USB2.0 DEVICE ATTACH !\n");
				gDeviceUsbType = USB_U20_SPEED;
				gDeviceConnectstatus = USB_INT_CONNECT_U20;
				mDelaymS(100);
				usbssh_init();		//��ʼ��USB3.0�Ĵ���
				R8_UHOST_CTRL |= RB_UH_BUS_RESET;
				mDelaymS(200);
				R8_UHOST_CTRL &= ~RB_UH_BUS_RESET;
			}
			else
			{
				printf("USB2.0 DEVICE DEATTACH !\n");
			}
		}
	}	
}

/*******************************************************************************
* Function Name  : USBHS_IRQHandler
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void USBHS_IRQHandler (void)			//USBSS interrupt severice
{

}

/*******************************************************************************
* Function Name  : USBSS_IRQHandler
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void USBSS_IRQHandler (void)			//USBSSH interrupt severice
{
	
}

/*******************************************************************************
* Function Name  : LINK_IRQHandler
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void LINK_IRQHandler (void)			//USBSSH interrupt severice
{
	UINT32 temp;

	temp = USBSS->LINK_ERR_STATUS;
	if( USBSSH->LINK_INT_FLAG & LINK_RECOV_FLAG )
	{
		USBSSH->LINK_INT_FLAG = LINK_RECOV_FLAG;
		printf("link recovery, error status = %0x\n", temp);
	}
	if( USBSSH->LINK_INT_FLAG & LINK_INACT_FLAG )
	{

		USBSSH->LINK_INT_FLAG = LINK_INACT_FLAG;
		switch_pwr_mode(POWER_MODE_2);
		printf("link inactive, error status = %0x\n", temp>>16);
	}
	else if( USBSS->LINK_INT_FLAG & LINK_DISABLE_FLAG ) // GO DISABLED
	{
		USBSSH->LINK_INT_FLAG = LINK_DISABLE_FLAG;
		USBSS->LINK_CTRL = POWER_MODE_2;// GO RX DETECT
			printf("go disabled \n");
	}
	else if( USBSSH->LINK_INT_FLAG & LINK_RX_DET_FLAG )
	{
		USBSSH->LINK_INT_FLAG = LINK_RX_DET_FLAG;

		printf("link is det !\n\n");
		switch_pwr_mode(POWER_MODE_2);
	}
	else if( USBSSH->LINK_INT_FLAG & TERM_PRESENT_FLAG ) // term present , begin POLLING
	{
		USBSSH->LINK_INT_FLAG = TERM_PRESENT_FLAG;
		if( USBSS->LINK_STATUS & LINK_PRESENT )
		{
			switch_pwr_mode(POWER_MODE_2);
			USBSSH->LINK_CTRL |= POLLING_EN;
			printf("rx term present!\n\n");
		}
		else
		{
			USBSSH->LINK_INT_CTRL = 0;
			printf("link is disconnect !\n\n");
			gDeviceConnectstatus = USB_INT_DISCONNECT;
			gDeviceUsbType = 0;
		}
	}
	else if( USBSSH->LINK_INT_FLAG & LINK_TXEQ_FLAG ) // POLLING SHAKE DONE
	{
		tx_lmp_port = 1;
		USBSSH->LINK_INT_FLAG = LINK_TXEQ_FLAG;
		if( USBSS->LINK_STATUS & LINK_PRESENT )
		{
			while(USBSS->LINK_STATUS & LINK_RX_DETECT);
			switch_pwr_mode(POWER_MODE_0);
			printf("link is tx EQ !%0x\n\n", USBSS->LINK_STATUS);
		}
	}
	else if( USBSSH->LINK_INT_FLAG & LINK_RDY_FLAG ) // POLLING SHAKE DONE
	{
		USBSSH->LINK_INT_FLAG = LINK_RDY_FLAG;
		if( tx_lmp_port ) // LMP, TX PORT_CAP & RX PORT_CAP
		{
			tx_lmp_port = 0;
			usbssh_lmp_init();
			printf("lmp rtx successfully !\n\n");
			enum_ready = 1;
			gDeviceConnectstatus = USB_INT_CONNECT;
			gDeviceUsbType = USB_U30_SPEED;
		}

	}

}

/*******************************************************************************
* Function Name  : usbssh_init_reset
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void usbssh_init_reset( void )
{
	USBSSH->LINK_INT_CTRL = 0;	// disable all link int enable
	USBSSH->LINK_CTRL = POWER_MODE_3 | GO_DISABLED;		// change U3-PHY enter P3 & link disabled
	USBSSH->LINK_CFG = PIPE_RESET | LFPS_RX_PD | LPM_EN | DEEMPH_CFG;// reset U3-PHY
	USBSSH->USB_CONTROL = USB_ALL_CLR | USB_FORCE_RST; // reset link layer
	USBSSH->UH_TX_CTRL = 0;
	USBSSH->UH_RX_CTRL = 0;
}

/*******************************************************************************
* Function Name  : usbssh_init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void usbssh_init ()	// USBSS host initial
{	
	USBSSH->UH_TX_CTRL = 0;
	USBSSH->UH_RX_CTRL = 0;
	USBSSH->LINK_CFG = HP_PENDING | CFG_EQ_EN | DEEMPH_CFG | DOWN_FLAG;// downstream
	USBSSH->LINK_CTRL = POWER_MODE_2 | GO_DISABLED;		// change U3-PHY enter P2

	while( USBSSH->LINK_STATUS & LINK_BUSY ); 			// wait link_busy=0
	printf("power mode: P2 \n");
	USBSSH->USB_CONTROL = HOST_MODE | ITP_EN | INT_BUSY_EN | DMA_EN;// host_mode, release link reset
	USBSSH->UH_TX_CTRL = 0x0;
	USBSSH->USB_STATUS = USB_ACT_FLAG | USB_LMP_TX_FLAG | USB_LMP_RX_FLAG | USB_OV_FLAG;
	USBSSH->UH_TX_DMA = Usb_Tx_DMAaddr; 					// set tx dma address
	USBSSH->UH_RX_DMA = Usb_Rx_DMAaddr; 					// set rx dma address
	USBSSH->UEP_CFG = UH_T_EN | UH_R_EN;				// set HOST rx/tx enable
	printf("TERM ENABLE \n");
	USBSS->LINK_CFG |= TERM_EN; // term enable
	USBSS->LINK_INT_CTRL = LINK_DISABLE_IE | LINK_INACT_IE | LINK_RX_DET_IE | TERM_PRESENT_IE | LINK_TXEQ_IE | LINK_RDY_IE;

	USBSS->LINK_CTRL = POWER_MODE_2; // GO RX DETECT
}




