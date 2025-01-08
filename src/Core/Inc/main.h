/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DAC_DB8_Pin GPIO_PIN_8
#define DAC_DB8_GPIO_Port GPIOI
#define DAC_DB9_Pin GPIO_PIN_9
#define DAC_DB9_GPIO_Port GPIOI
#define DAC_DB10_Pin GPIO_PIN_10
#define DAC_DB10_GPIO_Port GPIOI
#define DAC_DB11_Pin GPIO_PIN_11
#define DAC_DB11_GPIO_Port GPIOI
#define DAC_NCS_Pin GPIO_PIN_1
#define DAC_NCS_GPIO_Port GPIOA
#define STATUS_LED_R_Pin GPIO_PIN_2
#define STATUS_LED_R_GPIO_Port GPIOH
#define STATUS_LED_G_Pin GPIO_PIN_3
#define STATUS_LED_G_GPIO_Port GPIOH
#define STATUS_LED_B_Pin GPIO_PIN_4
#define STATUS_LED_B_GPIO_Port GPIOH
#define DAC_DB0_Pin GPIO_PIN_0
#define DAC_DB0_GPIO_Port GPIOI
#define DAC_DB1_Pin GPIO_PIN_1
#define DAC_DB1_GPIO_Port GPIOI
#define DAC_DB2_Pin GPIO_PIN_2
#define DAC_DB2_GPIO_Port GPIOI
#define DAC_DB3_Pin GPIO_PIN_3
#define DAC_DB3_GPIO_Port GPIOI
#define SD_NCD_Pin GPIO_PIN_3
#define SD_NCD_GPIO_Port GPIOD
#define DAC_DB4_Pin GPIO_PIN_4
#define DAC_DB4_GPIO_Port GPIOI
#define DAC_DB5_Pin GPIO_PIN_5
#define DAC_DB5_GPIO_Port GPIOI
#define DAC_DB6_Pin GPIO_PIN_6
#define DAC_DB6_GPIO_Port GPIOI
#define DAC_DB7_Pin GPIO_PIN_7
#define DAC_DB7_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
