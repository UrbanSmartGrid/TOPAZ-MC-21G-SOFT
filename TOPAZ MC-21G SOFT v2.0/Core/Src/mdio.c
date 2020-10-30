#include "def.h"


/******************************************************************************/
static inline void delay_MDIO(uint8_t del)
{
	while(del--);
}


/******************************************************************************/
static inline void output_MDIO(uint32_t value, uint32_t length)
{
	for(value <<= (32-length); length; value <<= 1, length--)
	{
		if(value & 0x80000000)
			MDIO_H;
		else
			MDIO_L;

		delay_MDIO(0);
		MDC_H;
		delay_MDIO(1);
		MDC_L;
	}
}


/******************************************************************************/
static inline uint32_t input_MDIO(void)
{
	uint32_t i;
	uint32_t value=0;


	for(i=0; i<16; i++)
	{
		value <<= 1;

		MDC_H;
		delay_MDIO(0);
		MDC_L;

		if(MDIO_GPIO_Port->IDR & MDIO_Pin)
			value |= 1;
	}


	return value;
}


/******************************************************************************/
static inline void turnaround_MDIO()
{
	// set MDIO as input
	MDIO_GPIO_Port->MODER &= ~(1 << MODER_MDIO_OFFSET);

	MDC_H;
	delay_MDIO(1);
	MDC_L;
	delay_MDIO(0);
}


///******************************************************************************/
uint32_t read_MDIO(uint8_t phy_address, uint8_t reg_address)
{
	static uint32_t value = 0;


	// set MDIO as output
	MDIO_GPIO_Port->MODER |= (1 << MODER_MDIO_OFFSET);

	// sync
	output_MDIO(0xFFFFFFFF, 32);
	// start code (01) + read command (10)
	output_MDIO(0x06, 4);
	// write PHY address
	output_MDIO(phy_address, 5);
	// write REG address
	output_MDIO(reg_address, 5);
	// z
	turnaround_MDIO();
	// read REG
	value = input_MDIO();
	// z
	turnaround_MDIO();

	return value;
}


/******************************************************************************/
void write_MDIO(uint8_t phy_address, uint8_t reg_address, uint16_t value)
{
	// set MDIO as output
	MDIO_GPIO_Port->MODER |= (1 << MODER_MDIO_OFFSET);

	// sync
	output_MDIO(0xFFFFFFFF, 32);
	// start code (01) + write command (01)
	output_MDIO(0x05, 4);
	// write PHY address
	output_MDIO(phy_address, 5);
	// write REG address
	output_MDIO(reg_address, 5);
	// 10
	output_MDIO(0x02, 2);
	// write the data
	output_MDIO(value, 16);
	// z
	turnaround_MDIO();
}


/******************************************************************************/
uint32_t read_Indirect(uint8_t phy_address, uint16_t reg_address)
{
	write_MDIO(phy_address, 0x0D, 0x001F);
	write_MDIO(phy_address, 0x0E, reg_address);
	write_MDIO(phy_address, 0x0D, 0x401F);

	return read_MDIO(phy_address, 0x0E);
}


/******************************************************************************/
void write_Indirect(uint8_t phy_address, uint16_t reg_address, uint16_t value)
{
	write_MDIO(phy_address, 0x0D, 0x001F);
	write_MDIO(phy_address, 0x0E, reg_address);
	write_MDIO(phy_address, 0x0D, 0x401F);
	write_MDIO(phy_address, 0x0E, value);
}
