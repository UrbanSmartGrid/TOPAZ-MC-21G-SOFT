#include "def.h"


SFP_PACKET	sfp_packet;

GPIO_TypeDef*	port_SCL;
uint16_t		pin_SCL;
GPIO_TypeDef*	port_SDA;
uint16_t		pin_SDA;


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
				// ñîáûòèå "ÂÑÒÀÂËÅÍ ÌÎÄÓËÜ SFP ÊÀÍÀË 1"
				system_events.event_sfp_inserted_ch1 = TRUE;
			}
			else
				system_status.ch1_sfp_presence = ABSENT;	// ëîæíûé ñèãíàë "ÂÑÒÀÂËÅÍ ÌÎÄÓËÜ SFP ÊÀÍÀË 1"
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
				// ñîáûòèå "ÌÎÄÓËÜ SFP ÊÀÍÀË 1 ÈÇÂËÅ×¨Í"
				system_events.event_sfp_removed_ch1 = TRUE;
			}
			else
				system_status.ch1_sfp_presence = PRESENT;	// ëîæíûé ñèãíàë "ÌÎÄÓËÜ SFP ÊÀÍÀË 1 ÈÇÂËÅ×¨Í"
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
				// ñîáûòèå "ÂÑÒÀÂËÅÍ ÌÎÄÓËÜ SFP ÊÀÍÀË 2"
				system_events.event_sfp_inserted_ch2 = TRUE;
			}
			else
				system_status.ch2_sfp_presence = ABSENT;	// ëîæíûé ñèãíàë "ÂÑÒÀÂËÅÍ ÌÎÄÓËÜ SFP ÊÀÍÀË 2"
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
				// ñîáûòèå "ÌÎÄÓËÜ SFP ÊÀÍÀË 2 ÈÇÂËÅ×¨Í"
				system_events.event_sfp_removed_ch2 = TRUE;
			}
			else
				system_status.ch2_sfp_presence = PRESENT;	// ëîæíûé ñèãíàë "ÌÎÄÓËÜ SFP ÊÀÍÀË 2 ÈÇÂËÅ×¨Í"
		}
		break;
	}
}


/******************************************************************************/
ErrorStatus SFP_WriteData(uint8_t bus_address, uint8_t reg_address, uint8_t *data, uint8_t data_length, CHANNEL channel)
{
	sfp_packet.bus_address_operation = bus_address;
	SFP_OPERATION_WRITE(sfp_packet.bus_address_operation);
	sfp_packet.register_address = reg_address;
	sfp_packet.payload = data;
	sfp_packet.payload_length = data_length;
	
	switch(channel)
	{
		//----------------------------------------//
		case CHANNEL1:
		{
			port_SCL	= SFP_SCL_CH1_GPIO_Port;
			pin_SCL		= SFP_SCL_CH1_Pin;
			port_SDA	= SFP_SDA_CH1_GPIO_Port;
			pin_SDA		= SFP_SDA_CH1_Pin;
		}
		break;
		//----------------------------------------//
		case CHANNEL2:
		{
			port_SCL	= SFP_SCL_CH2_GPIO_Port;
			pin_SCL		= SFP_SCL_CH2_Pin;
			port_SDA	= SFP_SDA_CH2_GPIO_Port;
			pin_SDA		= SFP_SDA_CH2_Pin;
		}
		break;
		//----------------------------------------//
		default:
			return ERROR;
		break;
	}

	
	// START I2C TRANSACTION
	
	
	
	
	return SUCCESS;
}


