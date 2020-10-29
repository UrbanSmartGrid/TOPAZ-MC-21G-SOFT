/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EXT_MDIO_Pin GPIO_PIN_0
#define EXT_MDIO_GPIO_Port GPIOA
#define EXT_MDC_Pin GPIO_PIN_1
#define EXT_MDC_GPIO_Port GPIOA
#define PHY_MDC_Pin GPIO_PIN_2
#define PHY_MDC_GPIO_Port GPIOA
#define PHY_MDIO_Pin GPIO_PIN_3
#define PHY_MDIO_GPIO_Port GPIOA
#define CPU_PHY_RESET_CH1_Pin GPIO_PIN_4
#define CPU_PHY_RESET_CH1_GPIO_Port GPIOA
#define CPU_PHY_RESET_CH2_Pin GPIO_PIN_5
#define CPU_PHY_RESET_CH2_GPIO_Port GPIOA
#define OUT_PHY_RESET_CH1_Pin GPIO_PIN_6
#define OUT_PHY_RESET_CH1_GPIO_Port GPIOA
#define OUT_PHY_RESET_CH2_Pin GPIO_PIN_7
#define OUT_PHY_RESET_CH2_GPIO_Port GPIOA
#define REGIM0_Pin GPIO_PIN_0
#define REGIM0_GPIO_Port GPIOB
#define REGIM1_Pin GPIO_PIN_1
#define REGIM1_GPIO_Port GPIOB
#define SFP_PRESENCE_CH1_Pin GPIO_PIN_2
#define SFP_PRESENCE_CH1_GPIO_Port GPIOB
#define E2PROM_SCL_Pin GPIO_PIN_13
#define E2PROM_SCL_GPIO_Port GPIOB
#define E2PROM_SDA_Pin GPIO_PIN_14
#define E2PROM_SDA_GPIO_Port GPIOB
#define FIBER_SD_CH1_Pin GPIO_PIN_10
#define FIBER_SD_CH1_GPIO_Port GPIOA
#define FIBER_SD_CH2_Pin GPIO_PIN_15
#define FIBER_SD_CH2_GPIO_Port GPIOA
#define SFP_SCL_CH1_Pin GPIO_PIN_3
#define SFP_SCL_CH1_GPIO_Port GPIOB
#define SFP_SDA_CH1_Pin GPIO_PIN_4
#define SFP_SDA_CH1_GPIO_Port GPIOB
#define SFP_TX_ENABLE_CH1_Pin GPIO_PIN_5
#define SFP_TX_ENABLE_CH1_GPIO_Port GPIOB
#define SFP_PRESENCE_CH2_Pin GPIO_PIN_6
#define SFP_PRESENCE_CH2_GPIO_Port GPIOB
#define SFP_SCL_CH2_Pin GPIO_PIN_7
#define SFP_SCL_CH2_GPIO_Port GPIOB
#define SFP_SDA_CH2_Pin GPIO_PIN_8
#define SFP_SDA_CH2_GPIO_Port GPIOB
#define SFP_TX_ENABLE_CH2_Pin GPIO_PIN_9
#define SFP_TX_ENABLE_CH2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
