/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define tol_BUT1 200
#define tol_BUT2 200

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

volatile uint8_t DispCnt;
volatile uint8_t Disp[4];
volatile uint16_t RGB_cnt;
volatile uint32_t ltBUT1 ;
volatile uint32_t ltBUT2;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM11_Init(void);
/* USER CODE BEGIN PFP */
void ustaw_na_1(){
	HAL_GPIO_WritePin(GPIOC, A_Pin | B_Pin| C_Pin| D_Pin | E_Pin | F_Pin | G_Pin | DP_Pin, GPIO_PIN_SET);
}


void wyswietl(int liczba){
	ustaw_na_1();
	switch(liczba) {
		case 0:
		HAL_GPIO_WritePin(GPIOC, G_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOC, A_Pin | D_Pin | E_Pin | F_Pin | G_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, C_Pin | F_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOC, E_Pin | F_Pin, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOC, E_Pin | D_Pin | A_Pin, GPIO_PIN_RESET);
		break;//
	case 5:
		HAL_GPIO_WritePin(GPIOC, B_Pin | E_Pin, GPIO_PIN_RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOC, E_Pin | F_Pin | G_Pin | D_Pin, GPIO_PIN_RESET);
		break;
	case 8:
		break;
	case 9:
		HAL_GPIO_WritePin(GPIOC, E_Pin, GPIO_PIN_RESET);
		break;
	}
}

void zmiana_liczby(uint16_t liczba){

	Disp[3] = (uint8_t)(liczba/1000);
	liczba = liczba - Disp[3]*1000;
	Disp[2] = (uint8_t)(liczba/100);
	liczba = liczba - Disp[2]*100;
	Disp[1] = (uint8_t)(liczba/10);
	liczba = liczba - Disp[1]*10;
	Disp[0] = (uint8_t)(liczba);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{


	if (htim -> Instance == TIM11){
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	switch(DispCnt) {

		case 0:
			wyswietl(Disp[3]);
			if (RGB_cnt > 999)
				HAL_GPIO_WritePin(COM1_GPIO_Port, COM1_Pin, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(COM1_GPIO_Port, COM1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(COM1_GPIO_Port, COM2_Pin|COM3_Pin|COM4_Pin, GPIO_PIN_SET);
			break;
		case 1:
			wyswietl(Disp[2]);
			if (RGB_cnt > 99)
				HAL_GPIO_WritePin(COM1_GPIO_Port, COM2_Pin, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(COM1_GPIO_Port, COM1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(COM1_GPIO_Port, COM1_Pin|COM3_Pin|COM4_Pin, GPIO_PIN_SET);
			break;
		case 2:
			wyswietl(Disp[1]);
			if (RGB_cnt > 9)
				HAL_GPIO_WritePin(COM1_GPIO_Port, COM3_Pin, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(COM1_GPIO_Port, COM3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(COM1_GPIO_Port, COM2_Pin|COM1_Pin|COM4_Pin, GPIO_PIN_SET);
			break;
		case 3:
			wyswietl(Disp[0]);
			HAL_GPIO_WritePin(COM1_GPIO_Port, COM4_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(COM1_GPIO_Port, COM2_Pin|COM1_Pin|COM3_Pin, GPIO_PIN_SET);
			break;


		}

		DispCnt++;
		if (DispCnt > 3){
			DispCnt = 0;
		}
	}

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){



	if(GPIO_Pin == BUT_1_Pin){
		uint32_t ctBUT1 = HAL_GetTick();
		uint32_t diff1 = ctBUT1 - ltBUT1;

		if ((diff1 > tol_BUT1) && (HAL_GPIO_ReadPin(BUT_1_GPIO_Port, BUT_1_Pin) == 0)){
			RGB_cnt++;
			ltBUT1 = ctBUT1;}

		if (RGB_cnt >9999){
			RGB_cnt = 0;
		}
	}

	if(GPIO_Pin == BUT_2_Pin){
		uint32_t ctBUT2 = HAL_GetTick();
		uint32_t diff2 = ctBUT2 - ltBUT2;

			if (RGB_cnt == 0){
				RGB_cnt = 9999;
			}
			else{
				if (diff2 > tol_BUT2 && (HAL_GPIO_ReadPin(BUT_2_GPIO_Port, BUT_2_Pin) == 0)){
					ltBUT2 = ctBUT2;
					RGB_cnt --;}
			}
		}
}


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
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim11);

//  zmiana_liczby(3241);
  ltBUT1 = HAL_GetTick();
  ltBUT2 = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {

//	  HAL_Delay(1000);
//	  	  Disp[0] = Disp[0] +1;
//	  	  if (Disp[0] > 9){
//	  		  Disp[0] = 0;
//	  		  Disp[1] = Disp[1] + 1;
//	  		  if (Disp[1] > 9){
//	  		  		  Disp[1] = 0;
//	  		  		  Disp[2] = Disp[2] + 1;
//	  		  		if (Disp[2] > 9){
//	  		  				  Disp[2] = 0;
//	  		  				  Disp[3] = Disp[3] + 1;
//	  		  				if (Disp[3] > 9){
//	  		  						  Disp[0] = 0;
//	  		  						  Disp[1] = 0;
//	  		  						  Disp[2] = 0;
//	  		  						  Disp[3] = 0;
//	  		  				}}}
//	  	  }
//
//
//
//
//
//
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  zmiana_liczby(RGB_cnt);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 8399;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 9;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

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
  huart2.Init.BaudRate = 115200;
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
  HAL_GPIO_WritePin(GPIOC, G_Pin|D_Pin|E_Pin|C_Pin
                          |B_Pin|F_Pin|A_Pin|DP_Pin
                          |COM4_Pin|COM3_Pin|COM2_Pin|COM1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : G_Pin D_Pin E_Pin C_Pin
                           B_Pin F_Pin A_Pin DP_Pin
                           COM4_Pin COM3_Pin COM2_Pin COM1_Pin */
  GPIO_InitStruct.Pin = G_Pin|D_Pin|E_Pin|C_Pin
                          |B_Pin|F_Pin|A_Pin|DP_Pin
                          |COM4_Pin|COM3_Pin|COM2_Pin|COM1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUT_1_Pin BUT_2_Pin */
  GPIO_InitStruct.Pin = BUT_1_Pin|BUT_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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
