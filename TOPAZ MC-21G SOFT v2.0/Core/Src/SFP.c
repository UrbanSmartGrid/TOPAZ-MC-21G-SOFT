#include "def.h"



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
				// ������� "�������� ������ SFP ����� 1"
				system_events.event_sfp_inserted_ch1 = TRUE;
			}
			else
				system_status.ch1_sfp_presence = ABSENT;	// ������ ������ "�������� ������ SFP ����� 1"
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
				// ������� "������ SFP ����� 1 �����ר�"
				system_events.event_sfp_removed_ch1 = TRUE;
			}
			else
				system_status.ch1_sfp_presence = PRESENT;	// ������ ������ "������ SFP ����� 1 �����ר�"
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
				// ������� "�������� ������ SFP ����� 2"
				system_events.event_sfp_inserted_ch2 = TRUE;
			}
			else
				system_status.ch2_sfp_presence = ABSENT;	// ������ ������ "�������� ������ SFP ����� 2"
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
				// ������� "������ SFP ����� 2 �����ר�"
				system_events.event_sfp_removed_ch2 = TRUE;
			}
			else
				system_status.ch2_sfp_presence = PRESENT;	// ������ ������ "������ SFP ����� 2 �����ר�"
		}
		break;
	}
}