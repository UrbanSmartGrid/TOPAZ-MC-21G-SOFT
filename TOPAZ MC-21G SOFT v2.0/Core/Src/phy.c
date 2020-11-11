#include "def.h"



/******************************************************************************/
ErrorStatus CheckPHYPresence(OPERATING_MODE op_mode)
{
	uint16_t reg_id1, reg_id2;
	
	
//	reg_id1 = read_MDIO(OUT_PHY_ADR_CH1, PHY_REG_ID1);
//	reg_id2 = read_MDIO(OUT_PHY_ADR_CH1, PHY_REG_ID2);
//
//	reg_id1 = read_MDIO(OUT_PHY_ADR_CH2, PHY_REG_ID1);
//	reg_id2 = read_MDIO(OUT_PHY_ADR_CH2, PHY_REG_ID2);
//	
//	if(op_mode == MODE0)
//		return SUCCESS;
		
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