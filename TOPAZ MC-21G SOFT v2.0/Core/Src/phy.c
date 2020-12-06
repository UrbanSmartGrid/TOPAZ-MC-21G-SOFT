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

void CheckCopperLinkStatus(void)
{
	uint16_t link_status;
	
	//------------------- CHANNEL 1 -------------------//
	// Link Status: This is latch low and needs to be read twice for valid link up
	if(read_MDIO(OUT_PHY_ADR_CH1, DP83869_REG_BMSR) & LINK_STS1_MASK)
	{
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
	// Link Status: This is latch low and needs to be read twice for valid link up
	if(read_MDIO(OUT_PHY_ADR_CH2, DP83869_REG_BMSR) & LINK_STS1_MASK)
	{
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


/******************************************************************************/
void ConfigureMediaConvertor100M(uint8_t phy_adr)
{
	write_Indirect(phy_adr, INDIRRECT_REG_OPMODE, 0x05);
	write_MDIO(phy_adr, 0x00, 0x1140);
	write_MDIO(phy_adr, DP83869_REG_ANAR, 0x0181);
	write_MDIO(phy_adr, DP83869_REG_GEN_CFG1, 0x0000);
	write_MDIO(phy_adr, 0x10, 0x5048);
	write_MDIO(phy_adr, 0x00, 0x1140 | (0x0001<<9));
}


/******************************************************************************/
void ConfigureMediaConvertor1000M(uint8_t phy_adr)
{
	write_Indirect(phy_adr, INDIRRECT_REG_OPMODE, 0x04);
	write_MDIO(phy_adr, 0x00, 0x1140);
	write_MDIO(phy_adr, 0x10, 0x5048);
//	write_Indirect(phy_adr, INDIRRECT_REG_FX_CTRL, 0x1140);	
	write_Indirect(phy_adr, INDIRRECT_REG_FX_CTRL, 0x0140);	
	write_MDIO(phy_adr, 0x00, 0x1140 | (0x0001<<9));
}


/******************************************************************************/
void ConfigurePHY(CHANNEL channel)
{
	uint8_t		phy_out_adr;
	uint8_t		phy_cpu_adr;
	SFP_TYPE	sfp_type;
	
	
	switch(channel)
	{
		case CHANNEL1:
		{
			phy_out_adr	= OUT_PHY_ADR_CH1; 
			phy_cpu_adr	= CPU_PHY_ADR_CH1;
			sfp_type	= system_status.ch1_sfp_type;
		} 
		break;
		case CHANNEL2:
		{
			phy_out_adr = OUT_PHY_ADR_CH2; 
			phy_cpu_adr = CPU_PHY_ADR_CH2;
			sfp_type	= system_status.ch2_sfp_type;
		} 
		break;
	}
	
	switch(system_status.op_mode)
	{
		//--------------------------------//
		case MODE0:	// media-convertor
		{
			if(sfp_type == SFP_100FX)
				ConfigureMediaConvertor100M(phy_out_adr);
			if(sfp_type == SFP_1000X)
				ConfigureMediaConvertor1000M(phy_out_adr);
		}			
		break;
	}
}


/******************************************************************************/
void CheckFiberLinkStatus(void)
{
//	uint16_t link_status;


	if(read_Indirect(OUT_PHY_ADR_CH1, INDIRRECT_REG_SERDES_SYNC_STS) & (0x0001<<8))
		printf("CH1 fiber sygnal\n");
	
	if(read_Indirect(OUT_PHY_ADR_CH2, INDIRRECT_REG_SERDES_SYNC_STS) & (0x0001<<8))
		printf("CH2 fiber sygnal\n");
}




































