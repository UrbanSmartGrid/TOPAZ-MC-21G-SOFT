#ifndef	__DEF_H__
#define	__DEF_H__


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "usbd_cdc_if.h"


typedef enum {FALSE=0, TRUE=1} BOOLEAN;


//  Режим работы, задаётся переключателями на плате:
//   - MODE 0 - два независимых медиаконвертора
//   - MODE 1 - два независимых порта с автодетектом
//   - MODE 2 - два независимых порта с оптическими трансиверами (SFP)
//   - MODE 3 - два независимых порта с разъёмами RJ-45
//   - MODE X - режим работы не определён, исключительная ситуация
typedef enum {MODE0=0, MODE1, MODE2, MODE3, MODEX} OPERATING_MODE;

// ошибки в работе программы или аппаратуры
typedef	enum {HW_ERROR=0, EXECUTE_ERROR} ERROR_TYPE;

// исключения, которые необходимо обрабатывать программно
// HW_EXCEPTION - критическая аппаратная ошибка, необходима аппаратная перезагрузка (через WDT)
typedef	enum {HW_EXCEPTION=0} EXCEPTION_TYPE;


// time-events structure
typedef	struct
{
	BOOLEAN	event_10ms;
	BOOLEAN	event_100ms;
	BOOLEAN	event_500ms;
	BOOLEAN	event_1000ms;
} TIME_EVENTS;


typedef struct
{
	BOOLEAN		event_sfp_inserted_ch1;
	BOOLEAN		event_sfp_inserted_ch2;
	BOOLEAN		event_sfp_removed_ch1;
	BOOLEAN		event_sfp_removed_ch2;
	BOOLEAN		ch1_linkup;
	BOOLEAN		ch2_linkup;
	BOOLEAN		ch1_linkdown;
	BOOLEAN		ch2_linkdown;
} SYSTEM_EVENTS;

// SFP presence state:
// ABSENT => отсутствует, начальное состояние или два последовательный сигнала ИЗВЛЕЧЁН (анти-дребезг)
// INSERTED => отсутствовало, но поступил сигнал ВСТАВЛЕН
// PRESENT => присутствует, повторное поступление сигнала ВСТАВЛЕН (подтверждение, анти-дребезг)
// REMOVED => присутствовало, но поступил сигнал ИЗВЛЕЧЁН
typedef enum {ABSENT=0, INSERTED, PRESENT, REMOVED} SFP_PRESENCE;

typedef enum {LINK_DOWN=0, LINK_UP_10M, LINK_UP_100M, LINK_UP_1000M} LINK_STATUS;
typedef enum {SFP_UNKNOWN, SFP_100FX, SFP_1000X} SFP_TYPE;

typedef struct
{
	OPERATING_MODE	op_mode;
	SFP_PRESENCE	ch1_sfp_presence;
	SFP_PRESENCE	ch2_sfp_presence;
	SFP_TYPE		ch1_sfp_type;
	SFP_TYPE		ch2_sfp_type;
	LINK_STATUS		ch1_link_status;
	LINK_STATUS		ch2_link_status;
	bool			ch1_fiber_signal;
	bool			ch2_fiber_signal;
} SYSTEM_STATUS;



// HW reset pins
#define	CPU_PHY_RESET_CH1_H		HAL_GPIO_WritePin(CPU_PHY_RESET_CH1_GPIO_Port, CPU_PHY_RESET_CH1_Pin, GPIO_PIN_SET);
#define	CPU_PHY_RESET_CH1_L		HAL_GPIO_WritePin(CPU_PHY_RESET_CH1_GPIO_Port, CPU_PHY_RESET_CH1_Pin, GPIO_PIN_RESET);
#define	CPU_PHY_RESET_CH2_H		HAL_GPIO_WritePin(CPU_PHY_RESET_CH2_GPIO_Port, CPU_PHY_RESET_CH2_Pin, GPIO_PIN_SET);
#define	CPU_PHY_RESET_CH2_L		HAL_GPIO_WritePin(CPU_PHY_RESET_CH2_GPIO_Port, CPU_PHY_RESET_CH2_Pin, GPIO_PIN_RESET);

#define	OUT_PHY_RESET_CH1_H		HAL_GPIO_WritePin(OUT_PHY_RESET_CH1_GPIO_Port, OUT_PHY_RESET_CH1_Pin, GPIO_PIN_SET);
#define	OUT_PHY_RESET_CH1_L		HAL_GPIO_WritePin(OUT_PHY_RESET_CH1_GPIO_Port, OUT_PHY_RESET_CH1_Pin, GPIO_PIN_RESET);
#define	OUT_PHY_RESET_CH2_H		HAL_GPIO_WritePin(OUT_PHY_RESET_CH2_GPIO_Port, OUT_PHY_RESET_CH2_Pin, GPIO_PIN_SET);
#define	OUT_PHY_RESET_CH2_L		HAL_GPIO_WritePin(OUT_PHY_RESET_CH2_GPIO_Port, OUT_PHY_RESET_CH2_Pin, GPIO_PIN_RESET);

// включить-выключить передатчик SFP
#define	SFP_TX_ENABLE_CH1		HAL_GPIO_WritePin(SFP_TX_ENABLE_CH1_GPIO_Port, SFP_TX_ENABLE_CH1_Pin, GPIO_PIN_RESET);
#define	SFP_TX_DISABLE_CH1		HAL_GPIO_WritePin(SFP_TX_ENABLE_CH1_GPIO_Port, SFP_TX_ENABLE_CH1_Pin, GPIO_PIN_SET);
#define	SFP_TX_ENABLE_CH2		HAL_GPIO_WritePin(SFP_TX_ENABLE_CH2_GPIO_Port, SFP_TX_ENABLE_CH2_Pin, GPIO_PIN_RESET);
#define	SFP_TX_DISABLE_CH2		HAL_GPIO_WritePin(SFP_TX_ENABLE_CH2_GPIO_Port, SFP_TX_ENABLE_CH2_Pin, GPIO_PIN_SET);


//#define	__MDIO_MASTER__
	
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


// PHY's register addresses
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

// DP83869
#define	DP83869_REG_BMCR				0x00
#define	DP83869_REG_BMSR				0x01
#define	DP83869_REG_ANAR				0x04
#define	DP83869_REG_GEN_CFG1			0x09
#define	DP83869_REG_PHY_STATUS			0x11
#define	INDIRRECT_REG_SERDES_SYNC_STS	0x4F
#define	INDIRRECT_REG_OPMODE			0x01DF
#define	INDIRRECT_REG_FIBER_STATUS		0x0C01
#define	INDIRRECT_REG_FX_CTRL			0x0C00



// PHY's ID
// DP83869
#define		DP83869_ID1				0x2000
#define		DP83869_ID2				0xA0F0
#define		DP83869_ID2_MASK		0xFFF0
// KSZ9031
#define		KSZ9031_ID1				0x0022
#define		KSZ9031_ID2				0x1620
#define		KSZ9031_ID2_MASK		0xFFF0



#define	__WDT__
#ifdef __WDT__
	// reset IWDT
	#define		WDT		HAL_IWDG_Refresh(&hiwdg);
#else
	#define		WDT
#endif	// __WDT__



//=======================  S O F T   I 2 C  =======================//
// clock
#define	CH1_SCL_H		SFP_SCL_CH1_GPIO_Port->BSRR = (uint32_t)SFP_SCL_CH1_Pin;
#define	CH1_SCL_L		SFP_SCL_CH1_GPIO_Port->BRR = (uint32_t)SFP_SCL_CH1_Pin;


#define	CH2_SCL_H		SFP_SCL_CH2_GPIO_Port->BSRR = (uint32_t)SFP_SCL_CH1_Pin;
#define	CH2_SCL_L		SFP_SCL_CH2_GPIO_Port->BRR = (uint32_t)SFP_SCL_CH1_Pin;


#define	SFP_SCL_H		port_SCL->BSRR = (uint32_t)pin_SCL;
#define	SFP_SCL_L		port_SCL->BRR = (uint32_t)pin_SCL;

// data
#define	CH1_SDA_H		SFP_SDA_CH1_GPIO_Port->BSRR = (uint32_t)SFP_SDA_CH1_Pin;
#define	CH1_SDA_L		SFP_SDA_CH1_GPIO_Port->BRR = (uint32_t)SFP_SDA_CH1_Pin;


#define	CH2_SDA_H		SFP_SDA_CH2_GPIO_Port->BSRR = (uint32_t)SFP_SDA_CH1_Pin;
#define	CH2_SDA_L		SFP_SDA_CH2_GPIO_Port->BRR = (uint32_t)SFP_SDA_CH1_Pin;


#define	SFP_SDA_H		port_SDA->BSRR = (uint32_t)pin_SDA;
#define	SFP_SDA_L		port_SDA->BRR = (uint32_t)pin_SDA;

// data direction
#define	CH1_SDA_INPUT_MASK	0xFFFFFCFF
#define	CH2_SDA_INPUT_MASK	0xFFFCFFFF

#define	CH1_SDA_OUTPUT_MASK	0x00000100
#define	CH2_SDA_OUTPUT_MASK	0x00010000


#define	CH1_SDA_INPUT		SFP_SDA_CH1_GPIO_Port->MODER &= CH1_SDA_INPUT_MASK;
#define	CH1_SDA_OUTPUT		SFP_SDA_CH1_GPIO_Port->MODER |= CH1_SDA_OUTPUT_MASK;

#define	CH2_SDA_INPUT		SFP_SDA_CH2_GPIO_Port->MODER &= CH2_SDA_INPUT_MASK;
#define	CH2_SDA_OUTPUT		SFP_SDA_CH2_GPIO_Port->MODER |= CH2_SDA_OUTPUT_MASK;

#define	SFP_SDA_INPUT		port_SDA->MODER &= pin_INPUT_MASK;
#define	SFP_SDA_OUTPUT		port_SDA->MODER |= pin_OUTPUT_MASK;

// SDA read
#define	SFP_SDA_IN_L		((port_SDA->IDR & pin_SDA) == 0)


#define	SFP_BUS_ADDRESS_LOW		0xA0
#define	SFP_BUS_ADDRESS_TOP		0xA2

#define	SFP_OPERATION_READ(bus_address)		bus_address |= 0x01
#define	SFP_OPERATION_WRITE(bus_address)	bus_address &= 0xFE

#define	SFP_PAYLOAD_MAX_LENGTH	256


typedef enum {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4} CHANNEL;


//=======================   C O N S O L E   =======================//
//#define	__DEBUG__
// write debug message to console
#ifdef __DEBUG__
	#define		ConsoleWrite(message)	{if(message != NULL)\
											CDC_Transmit_FS((uint8_t*)message, strlen(message));}
#else
	#define		ConsoleWrite(message)
#endif	// __DEBUG__

// console message templates
#define	_str_start_program		"START PROGRAMM EXECUTION!\n"
#define	_str_wrong_op_mode		"Operation mode isn't determined!\n"
#define	_str_phy_detect_error	"PHY`s set doesn't match to selected operation mode!\n"
#define	_str_sfp_detect_error	"SFP type isn't determined!\n"

//=================================================================//										
										

//======================= V A R    P R O T O S =======================//
// main.c
extern TIME_EVENTS		time_events;
extern SYSTEM_STATUS	system_status;
extern SYSTEM_EVENTS	system_events;

// SFP.c
extern GPIO_TypeDef*	port_SCL;
extern uint16_t			pin_SCL;
extern GPIO_TypeDef*	port_SDA;
extern uint16_t			pin_SDA;
extern uint32_t			pin_INPUT_MASK;
extern uint32_t			pin_OUTPUT_MASK;


//====================== F U N C    P R O T O S ======================//
// main.c
OPERATING_MODE DetermineOperatingMode(void);
void ResolveCriticalException(ERROR_TYPE error, char* message);

// mdio.c
uint32_t read_MDIO(uint8_t phy_address, uint8_t reg_address);
void write_MDIO(uint8_t phy_address, uint8_t reg_address, uint16_t value);
uint32_t read_Indirect(uint8_t phy_address, uint16_t reg_address);
void write_Indirect(uint8_t phy_address, uint16_t reg_address, uint16_t value);
uint16_t Marvell_ReadPortRegister(uint8_t chip_address, uint8_t int_dev_address, uint8_t int_reg_address);

// phy.c
ErrorStatus CheckPHYPresence(OPERATING_MODE op_mode);
void CheckCopperLinkStatus(void);
void CheckFiberLinkStatus(void);
void ConfigurePHY(CHANNEL channel);

// SFP.c
void CheckSFPPresence(void);
ErrorStatus SFP_WriteData(uint8_t bus_address, uint8_t reg_address, uint8_t *data, uint8_t data_length, CHANNEL channel);
ErrorStatus SFP_ReadData(uint8_t bus_address, uint8_t reg_address, uint8_t *data, uint8_t data_length, CHANNEL channel);
void i2c_clk_delay(uint16_t del);
void DefineSFPtype(CHANNEL channel);

// i2c_GPIO.c
uint8_t i2c_receive_byte_data(uint8_t address, uint8_t reg);
bool i2c_send_byte_data(uint8_t address, uint8_t reg, uint8_t data);

#endif	// __DEF_H__