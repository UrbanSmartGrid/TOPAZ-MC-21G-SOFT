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


/******************************************************************************/
inline ErrorStatus SFP_InitGPIO(CHANNEL channel)
{
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
			
			return SUCCESS;
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
			return SUCCESS;
		}
		break;
	}
	
	return ERROR;
}



/******************************************************************************/
ErrorStatus SFP_WriteDate(uint8_t bus_address, uint8_t reg_address, uint8_t *data, uint8_t data_length, CHANNEL channel)
{	
	if(SFP_InitGPIO(channel) == ERROR)
		return ERROR;
	
	for(uint8_t idx=0; idx<data_length; idx++)
	{
		if(i2c_send_byte_data(bus_address>>1, reg_address+idx, data[idx]) == false)
			return ERROR;
	}
	
	return SUCCESS;
}


/******************************************************************************/
ErrorStatus SFP_ReadData(uint8_t bus_address, uint8_t reg_address, uint8_t *data, uint8_t data_length, CHANNEL channel)
{
	if(SFP_InitGPIO(channel) == ERROR)
		return ERROR;
	
	
	for(uint8_t idx=0; idx<data_length; idx++)
		data[idx] = i2c_receive_byte_data(bus_address>>1, reg_address+idx);
	
	return SUCCESS;
}


/******************************************************************************/
void DefineSFPtype(CHANNEL channel)
{
	uint8_t	sfp_reg12;
	
	
	if(SFP_ReadData(SFP_BUS_ADDRESS_LOW, 12, &sfp_reg12, 1, channel) == ERROR)
	{
		if(channel == CHANNEL1)
			system_status.ch1_sfp_type = SFP_UNKNOWN;
		if(channel == CHANNEL2)
			system_status.ch2_sfp_type = SFP_UNKNOWN;		
	}
	
	
	if(sfp_reg12 == 0x02)
	{
		if(channel == CHANNEL1)
			system_status.ch1_sfp_type = SFP_100FX;
		if(channel == CHANNEL2)
			system_status.ch2_sfp_type = SFP_100FX;
		
		return;
	}		
	
	if(sfp_reg12 == 0x0D)
	{
		if(channel == CHANNEL1)
			system_status.ch1_sfp_type = SFP_1000X;
		if(channel == CHANNEL2)
			system_status.ch2_sfp_type = SFP_1000X;
		
		return;
	}
	
	
	if(channel == CHANNEL1)
		system_status.ch1_sfp_type = SFP_UNKNOWN;
	if(channel == CHANNEL2)
		system_status.ch2_sfp_type = SFP_UNKNOWN;
}




























