/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "def.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
TIME_EVENTS	time_events;

volatile uint16_t phy_reg;
const uint8_t fport_dev_adr[FPORT_NUMBER] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15,0x16, 0x17};
const uint8_t phy_dev_adr[FPORT_NUMBER] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,0x06, 0x07};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  
	HAL_Delay(250);
  
	// de-assert HW reset pins
	CPU_PHY_RESET_CH1_H
	CPU_PHY_RESET_CH2_H  
	OUT_PHY_RESET_CH1_H
	OUT_PHY_RESET_CH2_H	
		
		
	//!!!DEBUG
//	phy_reg = read_MDIO(CPU_PHY_ADR_CH1, PHY_REG_ID1);
//	phy_reg = read_MDIO(CPU_PHY_ADR_CH1, PHY_REG_ID2);
//	
//	phy_reg = read_MDIO(CPU_PHY_ADR_CH2, PHY_REG_ID1);
//	phy_reg = read_MDIO(CPU_PHY_ADR_CH2, PHY_REG_ID2);
//	
//	phy_reg = read_MDIO(OUT_PHY_ADR_CH1, PHY_REG_ID1);
//	phy_reg = read_MDIO(OUT_PHY_ADR_CH1, PHY_REG_ID2);
//	
//	phy_reg = read_MDIO(OUT_PHY_ADR_CH2, PHY_REG_ID1);
//	phy_reg = read_MDIO(OUT_PHY_ADR_CH2, PHY_REG_ID2);
		
	// read ID
	for(uint8_t port_idx=0; port_idx<FPORT_NUMBER; port_idx++)
		phy_reg = Marvell_ReadPortRegister(MARVELL_ADR_CHIP, fport_dev_adr[port_idx], REG_88E6097F_SWITCH_ID);
	
	for(uint8_t port_idx=0; port_idx<FPORT_NUMBER; port_idx++)
	{		
		phy_reg = Marvell_ReadPortRegister(MARVELL_ADR_CHIP, phy_dev_adr[port_idx], 0);
		phy_reg = Marvell_ReadPortRegister(MARVELL_ADR_CHIP, phy_dev_adr[port_idx], 1);
	}
		
//	phy_reg = read_MDIO(ADR_88E6097F, REG_CMD);
//	phy_reg = (LSB<<12) | (LSB<<11);
//	write_MDIO(ADR_88E6097F, REG_CMD, phy_reg);
//	phy_reg = 0;
//	phy_reg = read_MDIO(ADR_88E6097F, REG_CMD);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
