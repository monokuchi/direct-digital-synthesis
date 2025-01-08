/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DDS.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DAC_DATA_BUFFER_SIZE 16

#define SDRAM_START_ADDRESS 0xC0000000 // Look at Pg 802 of Reference Manual for memory mapping
#define SDRAM_SIZE 0x2000000 // Number of bytes in the SDRAM
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SD_HandleTypeDef hsd1;

TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_up;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FMC_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Update_StatusLED(const HAL_StatusTypeDef *status)
{
	// Turn on Status LED indicating that something went wrong
	if (status != HAL_OK)
	{
		HAL_GPIO_WritePin(STATUS_LED_R_GPIO_Port, STATUS_LED_R_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(STATUS_LED_G_GPIO_Port, STATUS_LED_G_Pin, GPIO_PIN_RESET);
	}
}

HAL_StatusTypeDef SDRAM_Init()
{
	/* Initializes the SDRAM to access read and write operations */

	FMC_SDRAM_CommandTypeDef Command;
	HAL_StatusTypeDef status;

    /* Step 1 and Step 2 already done in HAL_SDRAM_Init() */

    /* Step 3: Configure a clock configuration enable command */
    Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE; /* Set MODE bits to "001" */
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1; /* configure the Target Bank bits */
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;
    status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    if (status != HAL_OK) { return status; }

    /* Step 4: Insert 100 us minimum delay - Min HAL Delay is 1ms */
    HAL_Delay(1);

    /* Step 5: Configure a PALL (precharge all) command */
    Command.CommandMode            = FMC_SDRAM_CMD_PALL; /* Set MODE bits to "010" */
    status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    if (status != HAL_OK) { return status; }

    /* Step 6: Configure an Auto Refresh command */
    Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE; /* Set MODE bits to "011" */
    Command.AutoRefreshNumber      = 2;
    status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    if (status != HAL_OK) { return status; }

    /* Step 7: Program the external memory mode register */
    Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;/*set the MODE bits to "100" */
    Command.ModeRegisterDefinition =  (uint32_t)0 | 0<<3 | 2<<4 | 0<<7 | 1<<9;
    status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    if (status != HAL_OK) { return status; }

    /* Step 8: Set the refresh rate counter - refer to section SDRAM refresh timer register in RM0455 */
    /* Set the device refresh rate
	 * COUNT = [(SDRAM self refresh time / number of row) x  SDRAM CLK] – 20
	   		 = [(64ms/8192) * 80MHz] - 20 = 625 - 20 = 605 */
    status = HAL_SDRAM_ProgramRefreshRate(&hsdram1, 605);

    return status;
}

HAL_StatusTypeDef SDRAM_Read(SDRAM_HandleTypeDef *sdram)
{
	/* Performs a read operation on target SDRAM */
	HAL_StatusTypeDef status;

	return status;
}

HAL_StatusTypeDef SDRAM_Write(SDRAM_HandleTypeDef *sdram)
{
	/* Performs a write operation on target SDRAM */
	HAL_StatusTypeDef status;

	return status;
}

HAL_StatusTypeDef DAC_Init()
{
	/* Starts up the DAC clock and data signals */
	HAL_StatusTypeDef status;


	uint32_t dac_data_buffer[DAC_DATA_BUFFER_SIZE] = {0xFFFF0000, 0x00000001}; // Should toggle the first GPIOI pin on and off if DMA is in circular mode

	/* Prepare the DAC data signal to be sent on clock signal timer event via DMA transfers */
	status = HAL_DMA_Start(&hdma_tim2_up, (uint32_t) dac_data_buffer, (uint32_t) &(GPIOI->BSRR), DAC_DATA_BUFFER_SIZE);
	if (status != HAL_OK) { return status; }

	/* Start up the DAC clock signal */
	status = HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	if (status != HAL_OK) { return status; }
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 13); // Set duty cycle of PWM generation to 50%
    TIM2->DIER |= TIM_DIER_UDE; // Enable DMA on timer update event (counter overflow)


	return status;
}

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
  MX_DMA_Init();
  MX_FMC_Init();
  MX_SDMMC1_SD_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  if (SDRAM_Init() != HAL_OK) { Update_StatusLED(); } // Prepare the SDRAM for read/write operations

  if (DAC_Init() != HAL_OK) { Update_StatusLED(); } // Prepare the DAC clock and data signals

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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 3;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 24-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_3;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 6;
  SdramTiming.SelfRefreshTime = 3;
  SdramTiming.RowCycleDelay = 5;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, DAC_DB8_Pin|DAC_DB9_Pin|DAC_DB10_Pin|DAC_DB11_Pin
                          |DAC_DB0_Pin|DAC_DB1_Pin|DAC_DB2_Pin|DAC_DB3_Pin
                          |DAC_DB4_Pin|DAC_DB5_Pin|DAC_DB6_Pin|DAC_DB7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, STATUS_LED_R_Pin|STATUS_LED_G_Pin|STATUS_LED_B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DAC_DB8_Pin DAC_DB9_Pin DAC_DB10_Pin DAC_DB11_Pin
                           DAC_DB0_Pin DAC_DB1_Pin DAC_DB2_Pin DAC_DB3_Pin
                           DAC_DB4_Pin DAC_DB5_Pin DAC_DB6_Pin DAC_DB7_Pin */
  GPIO_InitStruct.Pin = DAC_DB8_Pin|DAC_DB9_Pin|DAC_DB10_Pin|DAC_DB11_Pin
                          |DAC_DB0_Pin|DAC_DB1_Pin|DAC_DB2_Pin|DAC_DB3_Pin
                          |DAC_DB4_Pin|DAC_DB5_Pin|DAC_DB6_Pin|DAC_DB7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : STATUS_LED_R_Pin STATUS_LED_G_Pin STATUS_LED_B_Pin */
  GPIO_InitStruct.Pin = STATUS_LED_R_Pin|STATUS_LED_G_Pin|STATUS_LED_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_NCD_Pin */
  GPIO_InitStruct.Pin = SD_NCD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SD_NCD_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
