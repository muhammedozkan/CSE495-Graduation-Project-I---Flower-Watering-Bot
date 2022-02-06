/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mylib.h"
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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t Rx_left[10];
uint8_t Rx_right[10];

int8_t left = 0;
int8_t right = 0;

int8_t previous = 0;
uint8_t flowerCode = CODE_R;
uint8_t leftRightForward = 0;



uint16_t findAmount(uint8_t flowerCode)
{
	for (int8_t i = 0; i < 3; i++) {
			if (amount[i].flowerCode == flowerCode)
				return amount[i].waterAmount;
		}
return 0;
}

void stop() {
	HAL_GPIO_WritePin(GPIOA,
			RIGHT_BACKWARD_Pin | LEFT_FORWARD_Pin | LEFT_BACKWARD_Pin
					| RIGHT_FORWARD_Pin, GPIO_PIN_RESET);
	left = 0;
	right = 0;

}

void forward() {
	stop();
	HAL_GPIO_WritePin(GPIOA, RIGHT_FORWARD_Pin | LEFT_FORWARD_Pin,GPIO_PIN_SET);
	left = 1;
	right = 1;
}

void turnLeft() {
	stop();
	HAL_GPIO_WritePin(RIGHT_FORWARD_GPIO_Port, RIGHT_FORWARD_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LEFT_BACKWARD_GPIO_Port, LEFT_BACKWARD_Pin, GPIO_PIN_SET);
	left = -1;
	right = 1;

	previous = -1;

}

void turnRight() {
	stop();
	HAL_GPIO_WritePin(LEFT_FORWARD_GPIO_Port, LEFT_FORWARD_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(RIGHT_BACKWARD_GPIO_Port, RIGHT_BACKWARD_Pin, GPIO_PIN_SET);
	left = 1;
	right = -1;

	previous = 1;

}
void backward() {
	stop();
	HAL_GPIO_WritePin(GPIOA, RIGHT_BACKWARD_Pin | LEFT_BACKWARD_Pin,GPIO_PIN_SET);
	left = -1;
	right = -1;

}

void readLeftSensor() {
	HAL_UART_Receive(&huart6, Rx_left, 10, 100);
}

void readRightSensor() {
	HAL_UART_Receive(&huart1, Rx_right, 10, 100);
}

void pumpON() {
	HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_RESET);
}

void pumpOFF() {
	HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_SET);
}

void pump(uint16_t time) {
	HAL_Delay(100);
	pumpON();
	HAL_Delay(time);
	pumpOFF();
	HAL_Delay(100);
}

void readSensor() {
	readLeftSensor();
	readRightSensor();
}

uint8_t karar(uint8_t code) {
	uint8_t rcount = 0;
	uint8_t lcount = 0;
	for (uint8_t i = 0; i < 10; ++i) // Using for loop we are initializing
			{
		if (Rx_right[i] == code) {
			rcount++;
		}

		if (Rx_left[i] == code) {
			lcount++;
		}

	}

	if (rcount > 0 && lcount > 0) {
		return 0;
	} else if (lcount > 0) {
		return 1;
	} else if (rcount > 0) {
		return 2;
	} else {
		return 3;
	}

}

void resetReadValue() {
	for (uint8_t i = 0; i < 10; ++i) {
		Rx_right[i] = Rx_left[i] = 0;
	}
}

uint8_t readProximity() {
	if (!HAL_GPIO_ReadPin(OBSTACLE_GPIO_Port, OBSTACLE_Pin)) {
		return 1;
	} else {
		return 0;
	}
}

void ara() {
	readSensor();
	leftRightForward = karar(flowerCode);

	switch (leftRightForward) {
	case 0:
		if (readProximity()) {
			HAL_Delay(100);
			stop();

			pump(findAmount(flowerCode));

			if (flowerCode == CODE_R)
				{flowerCode = CODE_G;}
			else if (flowerCode == CODE_G)
				{flowerCode = CODE_B;}
			else if (flowerCode == CODE_B)
				{flowerCode = CODE_R;
						backward();
						HAL_Delay(7500);
						stop();
						HAL_Delay(10000);}

		} else {
			forward();
		}
		break;

	case 1:

		turnLeft();
		break;

	case 2:

		turnRight();
		break;
	case 3:
		//obstacle detected
		if (readProximity()) {

			backward();
			HAL_Delay(5000);

		} else {
			if (previous > 0) {
				turnRight();

			} else {
				turnLeft();
			}

			//forward();
			//HAL_Delay(2500);
		}
		break;
	default:

		break;
	}

	resetReadValue();
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
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

	pumpOFF();
	HAL_Delay(100);
	memset(buffer, 0, sizeof(buffer));
	HAL_TIM_Base_Start_IT(&htim2);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		if (automode == 1) {
			ara();

		}

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 720;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 2400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_2;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 2400;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_2;
  huart6.Init.Parity = UART_PARITY_EVEN;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RIGHT_BACKWARD_Pin|LEFT_FORWARD_Pin|LEFT_BACKWARD_Pin|RIGHT_FORWARD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RIGHT_BACKWARD_Pin LEFT_FORWARD_Pin LEFT_BACKWARD_Pin RIGHT_FORWARD_Pin */
  GPIO_InitStruct.Pin = RIGHT_BACKWARD_Pin|LEFT_FORWARD_Pin|LEFT_BACKWARD_Pin|RIGHT_FORWARD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OBSTACLE_Pin */
  GPIO_InitStruct.Pin = OBSTACLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OBSTACLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WATER_PUMP_Pin */
  GPIO_InitStruct.Pin = WATER_PUMP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WATER_PUMP_GPIO_Port, &GPIO_InitStruct);

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
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
