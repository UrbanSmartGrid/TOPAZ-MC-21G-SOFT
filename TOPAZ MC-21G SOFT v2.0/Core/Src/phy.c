#include "def.h"



/******************************************************************************/
ErrorStatus CheckPHYPresence(OPERATING_MODE op_mode)
{
	uint16_t reg_id1, reg_id2;
	
	
	//----------------------------------------------------------------//
	// проверяем наличие PHY DP83869 (присутствуют во всех режимах)
	reg_id1 = read_MDIO(OUT_PHY_ADR_CH1, PHY_REG_ID1);
	reg_id2 = read_MDIO(OUT_PHY_ADR_CH1, PHY_REG_ID2);
	
	if((reg_id1 != DP83869_ID1) || ((reg_id2 & DP83869_ID2_MASK) != DP83869_ID2))
	   return ERROR;

	reg_id1 = read_MDIO(OUT_PHY_ADR_CH2, PHY_REG_ID1);
	reg_id2 = read_MDIO(OUT_PHY_ADR_CH2, PHY_REG_ID2);
	
	if((reg_id1 != DP83869_ID1) || ((reg_id2 & DP83869_ID2_MASK) != DP83869_ID2))
	   return ERROR;
	
	if(op_mode == MODE0)
		return SUCCESS;
	
	
	//----------------------------------------------------------------//
	// проверяем наличие PHY KSZ9031 (присутствуют во всех режимах, кроме MODE0)
	reg_id1 = read_MDIO(CPU_PHY_ADR_CH1, PHY_REG_ID1);
	reg_id2 = read_MDIO(CPU_PHY_ADR_CH1, PHY_REG_ID2);
	
	if((reg_id1 != KSZ9031_ID1) || ((reg_id2 & KSZ9031_ID2_MASK) != KSZ9031_ID2))
	   return ERROR;
	

	reg_id1 = read_MDIO(CPU_PHY_ADR_CH2, PHY_REG_ID1);
	reg_id2 = read_MDIO(CPU_PHY_ADR_CH2, PHY_REG_ID2);
	
	if((reg_id1 != KSZ9031_ID1) || ((reg_id2 & KSZ9031_ID2_MASK) != KSZ9031_ID2))
	   return ERROR;


	return SUCCESS;
}


/******************************************************************************/
#define	LINK_STS1_MASK		0x0004
#define	LINK_STATUS_2_MASK	(0x0001 << 10)
#define	LINK_AN_MASK		(0x0001 << 11)
#define	LINK_SPEED_MASK		0xC000
#define	LINK_SPEED_10M		0x0000
#define	LINK_SPEED_100M		0x4000
#define	LINK_SPEED_1000M	0x8000

void CheckLinkStatus(void)
{
	uint16_t link_status;
	
	//------------------- CHANNEL 1 -------------------//
	if(read_MDIO(OUT_PHY_ADR_CH1, DP83869_REG_BMSR) & LINK_STS1_MASK)
	{
		link_status = read_MDIO(OUT_PHY_ADR_CH1, DP83869_REG_PHY_STATUS);
		
		if((link_status & LINK_STATUS_2_MASK) && (link_status & LINK_AN_MASK))
		{
			if(system_status.ch1_link_status == LINK_DOWN)
			{
				system_events.ch1_linkup = TRUE;
				
				// определяем параметры линка
				switch(link_status & LINK_SPEED_MASK)
				{
					case LINK_SPEED_10M:
						system_status.ch1_link_status = LINK_UP_10M;
					break;
					case LINK_SPEED_100M:
						system_status.ch1_link_status = LINK_UP_100M;
					break;
					case LINK_SPEED_1000M:
						system_status.ch1_link_status = LINK_UP_1000M;
					break;
					default:
					// здесь нужно реализовать исключительную ситуацию и перезапустить Auto-Negotiation
					system_events.ch1_linkup = FALSE;
					break;
				}				
			}
		}
	}
	else
	{
		if(system_status.ch1_link_status != LINK_DOWN)
		{
			system_status.ch1_link_status = LINK_DOWN;
			system_events.ch1_linkdown = TRUE;
		}
	}
	
	
	//------------------- CHANNEL 2 -------------------//
	if(read_MDIO(OUT_PHY_ADR_CH2, DP83869_REG_BMSR) & LINK_STS1_MASK)
	{
		link_status = read_MDIO(OUT_PHY_ADR_CH2, DP83869_REG_PHY_STATUS);
		
		if((link_status & LINK_STATUS_2_MASK) && (link_status & LINK_AN_MASK))
		{
			if(system_status.ch2_link_status == LINK_DOWN)
			{
				system_events.ch2_linkup = TRUE;
				
				// определяем параметры линка
				switch(link_status & LINK_SPEED_MASK)
				{
					case LINK_SPEED_10M:
						system_status.ch2_link_status = LINK_UP_10M;
					break;
					case LINK_SPEED_100M:
						system_status.ch2_link_status = LINK_UP_100M;
					break;
					case LINK_SPEED_1000M:
						system_status.ch2_link_status = LINK_UP_1000M;
					break;
					default:
					// здесь нужно реализовать исключительную ситуацию и перезапустить Auto-Negotiation
					system_events.ch2_linkup = FALSE;
					break;
				}				
			}
		}
	}
	else
	{
		if(system_status.ch2_link_status != LINK_DOWN)
		{
			system_status.ch2_link_status = LINK_DOWN;
			system_events.ch2_linkdown = TRUE;
		}
	}
}




































