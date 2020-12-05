#include "def.h"


GPIO_TypeDef*	port_SCL;
uint16_t		pin_SCL;
GPIO_TypeDef*	port_SDA;
uint16_t		pin_SDA;

uint32_t	pin_INPUT_MASK;
uint32_t	pin_OUTPUT_MASK;


/******************************************************************************/
#define	I2C_DELAY_200KHz	10
#define	I2C_DELAY_100KHz	20
void i2c_clk_delay(uint16_t del)
{
	while(del--)
	{
		asm("NOP");
	}
}

/******************************************************************************/
void CheckSFPPresence(void)
{
	BOOLEAN sfp_ch1_present = FALSE;
	BOOLEAN sfp_ch2_present = FALSE;

	
	if(HAL_GPIO_ReadPin(SFP_PRESENCE_CH1_GPIO_Port, SFP_PRESENCE_CH1_Pin) == GPIO_PIN_RESET)
		sfp_ch1_present = TRUE;
	
	if(HAL_GPIO_ReadPin(SFP_PRESENCE_CH2_GPIO_Port, SFP_PRESENCE_CH2_Pin) == GPIO_PIN_RESET)
		sfp_ch2_present = TRUE;
	
	
	// FSM SFP channel 1
	switch(system_status.ch1_sfp_presence)
	{
		//----------------------//
		case ABSENT:
		{
			if(sfp_ch1_present == TRUE)
				system_status.ch1_sfp_presence = INSERTED;
		}
		break;
		//----------------------//
		case INSERTED:
		{
			if(sfp_ch1_present == TRUE)
			{
				system_status.ch1_sfp_presence = PRESENT;
				// ÒÓ·˚ÚËÂ "¬—“¿¬À≈Õ ÃŒƒ”À‹ SFP  ¿Õ¿À 1"
				system_events.event_sfp_inserted_ch1 = TRUE;
			}
			else
				system_status.ch1_sfp_presence = ABSENT;	// ÎÓÊÌ˚È ÒË„Ì‡Î "¬—“¿¬À≈Õ ÃŒƒ”À‹ SFP  ¿Õ¿À 1"
		}
		break;
		//----------------------//
		case PRESENT:
		{
			if(sfp_ch1_present == FALSE)
				system_status.ch1_sfp_presence = REMOVED;
		}
		break;
		//----------------------//
		case REMOVED:
		{
			if(sfp_ch1_present == FALSE)
			{
				system_status.ch1_sfp_presence = ABSENT;
				// ÒÓ·˚ÚËÂ "ÃŒƒ”À‹ SFP  ¿Õ¿À 1 »«¬À≈◊®Õ"
				system_events.event_sfp_removed_ch1 = TRUE;
			}
			else
				system_status.ch1_sfp_presence = PRESENT;	// ÎÓÊÌ˚È ÒË„Ì‡Î "ÃŒƒ”À‹ SFP  ¿Õ¿À 1 »«¬À≈◊®Õ"
		}
		break;
	}
	
	// FSM SFP channel 2
	switch(system_status.ch2_sfp_presence)
	{
		//----------------------//
		case ABSENT:
		{
			if(sfp_ch2_present == TRUE)
				system_status.ch2_sfp_presence = INSERTED;
		}
		break;
		//----------------------//
		case INSERTED:
		{
			if(sfp_ch2_present == TRUE)
			{
				system_status.ch2_sfp_presence = PRESENT;
				// ÒÓ·˚ÚËÂ "¬—“¿¬À≈Õ ÃŒƒ”À‹ SFP  ¿Õ¿À 2"
				system_events.event_sfp_inserted_ch2 = TRUE;
			}
			else
				system_status.ch2_sfp_presence = ABSENT;	// ÎÓÊÌ˚È ÒË„Ì‡Î "¬—“¿¬À≈Õ ÃŒƒ”À‹ SFP  ¿Õ¿À 2"
		}
		break;
		//----------------------//
		case PRESENT:
		{
			if(sfp_ch2_present == FALSE)
				system_status.ch2_sfp_presence = REMOVED;
		}
		break;
		//----------------------//
		case REMOVED:
		{
			if(sfp_ch2_present == FALSE)
			{
				system_status.ch2_sfp_presence = ABSENT;
				// ÒÓ·˚ÚËÂ "ÃŒƒ”À‹ SFP  ¿Õ¿À 2 »«¬À≈◊®Õ"
				system_events.event_sfp_removed_ch2 = TRUE;
			}
			else
				system_status.ch2_sfp_presence = PRESENT;	// ÎÓÊÌ˚È ÒË„Ì‡Î "ÃŒƒ”À‹ SFP  ¿Õ¿À 2 »«¬À≈◊®Õ"
		}
		break;
	}
}


///******************************************************************************/
//ErrorStatus SFP_TransmitByte(uint8_t byte)
//{
//	uint8_t current_bit_mask = 0x80;
//	ErrorStatus ret;
//	
//	
//	for(uint8_t i=0; i<8; i++)
//	{				
//		SFP_SCL_L
//		i2c_clk_delay(I2C_DELAY_200KHz);			
//		
//		// check current bit level
//		if(byte & current_bit_mask)
//			SFP_SDA_H
//		else
//			SFP_SDA_L
//				
//		current_bit_mask >>= 1;
//				
//		i2c_clk_delay(I2C_DELAY_200KHz);		
//		SFP_SCL_H
//		i2c_clk_delay(I2C_DELAY_100KHz);
//	}
//	
//	i2c_clk_delay(I2C_DELAY_200KHz);
//	SFP_SDA_H
//	SFP_SDA_INPUT
//		
//	SFP_SCL_L
//	i2c_clk_delay(I2C_DELAY_100KHz);
//	SFP_SCL_H
//	i2c_clk_delay(I2C_DELAY_200KHz);
//	
//	if(SFP_SDA_IN_L)
//		ret = SUCCESS;
//	else
//		ret = ERROR;
//		
//	i2c_clk_delay(I2C_DELAY_200KHz);
//	
//	SFP_SDA_OUTPUT
//			
//	return ret;
//}
//
//
//
///******************************************************************************/
//uint8_t SFP_ReceiveByte(void)
//{
//	uint8_t byte = 0x00;
//	
//	
//	SFP_SDA_H
//	SFP_SDA_INPUT
//			
//	for(uint8_t i=0; i<8; i++)
//	{				
//		SFP_SCL_L
//		i2c_clk_delay(I2C_DELAY_100KHz);
//		SFP_SCL_H
//		i2c_clk_delay(I2C_DELAY_200KHz);
//		
//		if(!SFP_SDA_IN_L)
//			byte |= (0x80>>i);
//		
//		i2c_clk_delay(I2C_DELAY_200KHz);
//	}
//			
//	SFP_SCL_L
//	i2c_clk_delay(I2C_DELAY_100KHz);
//	SFP_SCL_H
//	i2c_clk_delay(I2C_DELAY_100KHz);
//	
//		
//	SFP_SDA_OUTPUT
//		
//	return byte;
//}
//
//
//#define		SFP_STOP_CONDITION	SFP_SCL_L\
//								SFP_SDA_L\
//								i2c_clk_delay(I2C_DELAY_200KHz);\
//								SFP_SCL_H\
//								i2c_clk_delay(I2C_DELAY_200KHz);\
//								SFP_SDA_H


/******************************************************************************/
ErrorStatus SFP_WriteData(uint8_t bus_address, uint8_t reg_address, uint8_t *data, uint8_t data_length, CHANNEL channel)
{
	SFP_OPERATION_WRITE(bus_address);
	
	switch(channel)
	{
		//----------------------------------------//
		case CHANNEL1:
		{
			port_SCL	= SFP_SCL_CH1_GPIO_Port;
			pin_SCL		= SFP_SCL_CH1_Pin;
			port_SDA	= SFP_SDA_CH1_GPIO_Port;
			pin_SDA		= SFP_SDA_CH1_Pin;
			pin_INPUT_MASK	= CH1_SDA_INPUT_MASK;
			pin_OUTPUT_MASK	= CH1_SDA_OUTPUT_MASK;
		}
		break;
		//----------------------------------------//
		case CHANNEL2:
		{
			port_SCL	= SFP_SCL_CH2_GPIO_Port;
			pin_SCL		= SFP_SCL_CH2_Pin;
			port_SDA	= SFP_SDA_CH2_GPIO_Port;
			pin_SDA		= SFP_SDA_CH2_Pin;
			pin_INPUT_MASK	= CH2_SDA_INPUT_MASK;
			pin_OUTPUT_MASK	= CH2_SDA_OUTPUT_MASK;
		}
		break;
		//----------------------------------------//
		default:
			return ERROR;
		break;
	}

	
//	// START I2C TRANSACTION
//	SFP_SDA_H
//	SFP_SDA_OUTPUT
//	SFP_SCL_H
//		
//	i2c_clk_delay(I2C_DELAY_100KHz);
//	SFP_SDA_L
//	i2c_clk_delay(I2C_DELAY_200KHz);
//	SFP_SCL_L
//
//	if(SFP_TransmitByte(bus_address) == ERROR)
//	{
//		SFP_STOP_CONDITION
//			
//		return ERROR;
//	}
//	
//	if(SFP_TransmitByte(reg_address) == ERROR)
//	{
//		SFP_STOP_CONDITION
//			
//		return ERROR;
//	}
//	
//	
//	for(uint8_t data_idx=0; data_idx<data_length; data_idx++)
//	{
//		if(SFP_TransmitByte(data[data_idx]) == ERROR)
//		{
//			SFP_STOP_CONDITION
//				
//			return ERROR;
//		}
//	}
//	
//	
//	SFP_STOP_CONDITION
	
	
	return SUCCESS;
}


/******************************************************************************/
ErrorStatus SFP_ReadData(uint8_t bus_address, uint8_t reg_address, uint8_t *data, uint8_t data_length, CHANNEL channel)
{
	SFP_OPERATION_WRITE(bus_address);
	
	switch(channel)
	{
		//----------------------------------------//
		case CHANNEL1:
		{
			port_SCL	= SFP_SCL_CH1_GPIO_Port;
			pin_SCL		= SFP_SCL_CH1_Pin;
			port_SDA	= SFP_SDA_CH1_GPIO_Port;
			pin_SDA		= SFP_SDA_CH1_Pin;
			pin_INPUT_MASK	= CH1_SDA_INPUT_MASK;
			pin_OUTPUT_MASK	= CH1_SDA_OUTPUT_MASK;
		}
		break;
		//----------------------------------------//
		case CHANNEL2:
		{
			port_SCL	= SFP_SCL_CH2_GPIO_Port;
			pin_SCL		= SFP_SCL_CH2_Pin;
			port_SDA	= SFP_SDA_CH2_GPIO_Port;
			pin_SDA		= SFP_SDA_CH2_Pin;
			pin_INPUT_MASK	= CH2_SDA_INPUT_MASK;
			pin_OUTPUT_MASK	= CH2_SDA_OUTPUT_MASK;
		}
		break;
		//----------------------------------------//
		default:
			return ERROR;
		break;
	}

	
//	// START I2C TRANSACTION
//	SFP_SDA_H
//	SFP_SDA_OUTPUT
//	SFP_SCL_H
//		
//	i2c_clk_delay(I2C_DELAY_100KHz);
//	SFP_SDA_L
//	i2c_clk_delay(I2C_DELAY_200KHz);
//	SFP_SCL_L
//
//	if(SFP_TransmitByte(bus_address) == ERROR)
//	{
//		SFP_STOP_CONDITION
//			
//		return ERROR;
//	}
//	
//	if(SFP_TransmitByte(reg_address) == ERROR)
//	{
//		SFP_STOP_CONDITION
//			
//		return ERROR;
//	}
//	
//	
//	SFP_SCL_L
//	i2c_clk_delay(I2C_DELAY_100KHz);
//	
//	// START I2C TRANSACTION
//	SFP_SDA_H
//	SFP_SDA_OUTPUT
//	SFP_SCL_H
//		
//	i2c_clk_delay(I2C_DELAY_100KHz);
//	SFP_SDA_L
//	i2c_clk_delay(I2C_DELAY_200KHz);
//	SFP_SCL_L
//	
//	SFP_OPERATION_READ(bus_address);
//		
//	if(SFP_TransmitByte(bus_address) == ERROR)
//	{
//		SFP_STOP_CONDITION
//			
//		return ERROR;
//	}
//	
//	
//	for(uint8_t data_idx=0; data_idx<data_length; data_idx++)
//	{
//		data[data_idx] = SFP_ReceiveByte();
//	}
//	
//	
//	SFP_STOP_CONDITION
	
	
	return SUCCESS;
}


/******************************************************************************/
void DefineSFPtype(CHANNEL channel)
{
	uint8_t sfp_reg[1];
	
	
	
	switch(channel)
	{
		//----------------------------------------//
		case CHANNEL1:
		{
			port_SCL	= SFP_SCL_CH1_GPIO_Port;
			pin_SCL		= SFP_SCL_CH1_Pin;
			port_SDA	= SFP_SDA_CH1_GPIO_Port;
			pin_SDA		= SFP_SDA_CH1_Pin;
//			pin_INPUT_MASK	= CH1_SDA_INPUT_MASK;
//			pin_OUTPUT_MASK	= CH1_SDA_OUTPUT_MASK;
		}
		break;
		//----------------------------------------//
		case CHANNEL2:
		{
			port_SCL	= SFP_SCL_CH2_GPIO_Port;
			pin_SCL		= SFP_SCL_CH2_Pin;
			port_SDA	= SFP_SDA_CH2_GPIO_Port;
			pin_SDA		= SFP_SDA_CH2_Pin;
//			pin_INPUT_MASK	= CH2_SDA_INPUT_MASK;
//			pin_OUTPUT_MASK	= CH2_SDA_OUTPUT_MASK;
		}
		break;
		//----------------------------------------//
		default:
			return;
		break;
	}
	
	
	sfp_reg[0] = i2c_receive_byte_data(SFP_BUS_ADDRESS_LOW>>1, 12);

		
//	SFP_ReadData(SFP_BUS_ADDRESS_TOP, 127, &sfp_reg, 1, channel);
//	
	if(sfp_reg[0] == 0x55)
	{
		if(channel == CHANNEL1)
			system_status.ch1_sfp_type = SFP_100FX;
		if(channel == CHANNEL2)
			system_status.ch2_sfp_type = SFP_100FX;
		
		return;
	}		
//	
//	if(sfp_reg == 0xAA)
//	{
//		if(channel == CHANNEL1)
//			system_status.ch1_sfp_type = SFP_1000X;
//		if(channel == CHANNEL2)
//			system_status.ch2_sfp_type = SFP_1000X;
//		
//		return;
//	}
//	
//	if(channel == CHANNEL1)
//		system_status.ch1_sfp_type = SFP_UNKNOWN;
//	if(channel == CHANNEL2)
//		system_status.ch2_sfp_type = SFP_UNKNOWN;
}




























