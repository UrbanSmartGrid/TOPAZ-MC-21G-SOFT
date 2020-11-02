#ifndef	__DEF_H__
#define	__DEF_H__


#include <stdlib.h>
#include <stdio.h>
#include "main.h"



typedef enum {FALSE=0, TRUE=1} BOOLEAN;


// time-events structure
typedef	struct
{
	BOOLEAN	event_10ms;
	BOOLEAN	event_100ms;
	BOOLEAN	event_500ms;
	BOOLEAN	event_1000ms;
} TIME_EVENTS;



// HW reset pins
#define	CPU_PHY_RESET_CH1_H		HAL_GPIO_WritePin(CPU_PHY_RESET_CH1_GPIO_Port, CPU_PHY_RESET_CH1_Pin, GPIO_PIN_SET);
#define	CPU_PHY_RESET_CH1_L		HAL_GPIO_WritePin(CPU_PHY_RESET_CH1_GPIO_Port, CPU_PHY_RESET_CH1_Pin, GPIO_PIN_RESET);
#define	CPU_PHY_RESET_CH2_H		HAL_GPIO_WritePin(CPU_PHY_RESET_CH2_GPIO_Port, CPU_PHY_RESET_CH2_Pin, GPIO_PIN_SET);
#define	CPU_PHY_RESET_CH2_L		HAL_GPIO_WritePin(CPU_PHY_RESET_CH2_GPIO_Port, CPU_PHY_RESET_CH2_Pin, GPIO_PIN_RESET);

#define	OUT_PHY_RESET_CH1_H		HAL_GPIO_WritePin(OUT_PHY_RESET_CH1_GPIO_Port, OUT_PHY_RESET_CH1_Pin, GPIO_PIN_SET);
#define	OUT_PHY_RESET_CH1_L		HAL_GPIO_WritePin(OUT_PHY_RESET_CH1_GPIO_Port, OUT_PHY_RESET_CH1_Pin, GPIO_PIN_RESET);
#define	OUT_PHY_RESET_CH2_H		HAL_GPIO_WritePin(OUT_PHY_RESET_CH2_GPIO_Port, OUT_PHY_RESET_CH2_Pin, GPIO_PIN_SET);
#define	OUT_PHY_RESET_CH2_L		HAL_GPIO_WritePin(OUT_PHY_RESET_CH2_GPIO_Port, OUT_PHY_RESET_CH2_Pin, GPIO_PIN_RESET);



#define	__MDIO_MASTER__
	
#ifdef	__MDIO_MASTER__
	#define	MDC_GPIO_Port		EXT_MDC_GPIO_Port
	#define	MDIO_GPIO_Port		EXT_MDIO_GPIO_Port
	#define	MDC_Pin				EXT_MDC_Pin
	#define	MDIO_Pin			EXT_MDIO_Pin
	#define	MODER_MDIO_OFFSET	0

	#define	LSB16		0x0001

	#define	MARVELL_ADR_CHIP			0x06
	#define	MARVELL_REG_CMD				0x00
	#define	MARVELL_REG_DATA			0x01

	#define	REG_88E6097F_SWITCH_ID		0x03
	#define	REG_88E6097F_PHY_ID1		0x02
	#define	REG_88E6097F_PHY_ID2		0x03


	#define	FPORT_NUMBER	8		

#else
	#define	MDC_GPIO_Port		PHY_MDC_GPIO_Port
	#define	MDIO_GPIO_Port		PHY_MDIO_GPIO_Port
	#define	MDC_Pin				PHY_MDC_Pin
	#define	MDIO_Pin			PHY_MDIO_Pin
	#define	MODER_MDIO_OFFSET	6
#endif	// __MDIO_MASTER__

#define	MDC_H		MDC_GPIO_Port->BSRR = (uint32_t)MDC_Pin
#define	MDC_L		MDC_GPIO_Port->BRR = (uint32_t)MDC_Pin
#define	MDIO_H		MDIO_GPIO_Port->BSRR = (uint32_t)MDIO_Pin
#define	MDIO_L		MDIO_GPIO_Port->BRR = (uint32_t)MDIO_Pin


// PHY's MDIO-bus addresses
#define	CPU_PHY_ADR_CH1		0x01
#define	CPU_PHY_ADR_CH2		0x02
#define	OUT_PHY_ADR_CH1		0x03
#define	OUT_PHY_ADR_CH2		0x04


//// ?????? ????????? PHY
//#define	PHY_REG_BASIC_CONTROL	0x00
//#define	PHY_REG_BASIC_STATUS	0x01
#define	PHY_REG_ID1				0x02
#define	PHY_REG_ID2				0x03
//#define	PHY_REG_AN1				0x04
//#define	PHY_REG_AN2				0x05
//#define	PHY_REG_AN3				0x06
//#define	PHY_REG_AN4				0x07
//#define	PHY_REG_AN5				0x08
//#define	PHY_REG_CONTROL			0x10
//#define	PHY_STATUS_REG			0x11

//#define	PHY_REG_MII_CONTROL		0x14
//#define	PHY_REG_RXER_CNT		0x15
//#define	PHY_REG_INTERRUPT		0x1B
//#define	PHY_REG_LINK_MD			0x1D
//#define	PHY_REG_CONTROL1		0x1E
//#define	PHY_REG_CONTROL2		0x1F
//#define	INDIRRECT_REG_OPMODE		0x01DF
//#define	INDIRRECT_REG_FIBER_STATUS	0x0C01



//======================= V A R    P R O T O S =======================//
// main.c
extern TIME_EVENTS	time_events;


//====================== F U N C    P R O T O S ======================//
// mdio.c
uint32_t read_MDIO(uint8_t phy_address, uint8_t reg_address);
void write_MDIO(uint8_t phy_address, uint8_t reg_address, uint16_t value);
uint32_t read_Indirect(uint8_t phy_address, uint16_t reg_address);
void write_Indirect(uint8_t phy_address, uint16_t reg_address, uint16_t value);
uint16_t Marvell_ReadPortRegister(uint8_t chip_address, uint8_t int_dev_address, uint8_t int_reg_address);


#endif	// __DEF_H__