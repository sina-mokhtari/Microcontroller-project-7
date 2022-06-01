/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "LiquidCrystal.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum statusType {
	FIRST_NUM, SECOND_NUM, RESULT, ERROR0
};
enum operatorType {
	PLUS, MINUS, MULTIPLY, DIVISION
};
enum signType {
	POSITIVE, NEGATIVE
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define LCD_D8 GPIO_PIN_1
//#define LCD_D9 GPIO_PIN_2
//#define LCD_D10 GPIO_PIN_3
//#define LCD_D11 GPIO_PIN_4
//#define LCD_D12 GPIO_PIN_5
//#define LCD_D13 GPIO_PIN_6
//#define LCD_D14 GPIO_PIN_7
#define LCD_D8 GPIO_PIN_8
#define LCD_D9 GPIO_PIN_9
#define LCD_D10 GPIO_PIN_10
#define LCD_D11 GPIO_PIN_11
#define LCD_D12 GPIO_PIN_12
#define LCD_D13 GPIO_PIN_13
#define LCD_D14 GPIO_PIN_14

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim6;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
int lastExtiTime = 0;
enum statusType status = FIRST_NUM;
enum signType firstNumSign = POSITIVE;
enum operatorType operator;
int firstNumIdx = 0, secondNumIdx = 0;
int firstNumBuff[5] = { [0 ... 4] = -1 }, secondNumBuff[5] = { [0 ... 4] = -1 };
int currentRow = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void calcPrint(const char *c);
void calculate();
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
  MX_USB_PCD_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);
	LiquidCrystal(GPIOD, LCD_D8, LCD_D9, LCD_D10, LCD_D11, LCD_D12, LCD_D13,
	LCD_D14);
	begin(20, 4);

	print("0");

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 9599;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : MEMS_INT3_Pin MEMS_INT4_Pin */
  GPIO_InitStruct.Pin = MEMS_INT3_Pin|MEMS_INT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD4
                           PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2C1_SCL_Pin I2C1_SDA_Pin */
  GPIO_InitStruct.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 4 */
volatile int myNum;
volatile bool clicked = false;
char calcChar;
char str[20];
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		if (!clicked)
			return;

		if (status == ERROR0) {
			for (int i = 0; i < 4; i++)
				firstNumBuff[i] = secondNumBuff[i] = 0;
			firstNumIdx = secondNumIdx = 0;
			status = FIRST_NUM;
			firstNumSign = POSITIVE;
			currentRow = 0;
			clear();
			setCursor(0, 0);
			print("0");
			return;
		}

		switch (myNum) {
		case 1:
		case 2:
		case 3:
			if (status == FIRST_NUM) {
				if (firstNumIdx < 4) {
					firstNumBuff[firstNumIdx] = myNum;
					firstNumIdx++;
					sprintf(str, "%d", myNum);
					if (firstNumIdx == 1)
						setCursor(0, currentRow);
					print(str);
				}
			} else if (status == SECOND_NUM) {
				if (secondNumIdx < 4) {
					secondNumBuff[secondNumIdx] = myNum;
					secondNumIdx++;
					sprintf(str, "%d", myNum);
					print(str);
				}
			}

			break;
		case 5:
		case 6:
		case 7:
			if (status == FIRST_NUM) {
				if (firstNumIdx < 4) {
					firstNumBuff[firstNumIdx] = myNum - 1;
					firstNumIdx++;
					sprintf(str, "%d", myNum - 1);
					if (firstNumIdx == 1)
						setCursor(0, currentRow);
					print(str);
				}
			} else if (status == SECOND_NUM) {
				if (secondNumIdx < 4) {
					secondNumBuff[secondNumIdx] = myNum - 1;
					secondNumIdx++;
					sprintf(str, "%d", myNum - 1);
					print(str);
				}
			}

			break;
		case 9:
		case 10:
		case 11:
			if (status == FIRST_NUM) {
				if (firstNumIdx < 4) {
					firstNumBuff[firstNumIdx] = myNum - 2;
					firstNumIdx++;
					sprintf(str, "%d", myNum - 2);
					if (firstNumIdx == 1)
						setCursor(0, currentRow);
					print(str);
				}
			} else if (status == SECOND_NUM) {
				if (secondNumIdx < 4) {
					secondNumBuff[secondNumIdx] = myNum - 2;
					secondNumIdx++;
					sprintf(str, "%d", myNum - 2);
					print(str);
				}
			}

			break;
		case 14:
			if (status == FIRST_NUM) {
				if (firstNumIdx < 4 && firstNumIdx > 0) {
					firstNumBuff[firstNumIdx] = 0;
					firstNumIdx++;
					sprintf(str, "%d", 0);
					print(str);
				}
			} else if (status == SECOND_NUM) {
				if ((secondNumBuff[0] != 0 || secondNumIdx == 0) && secondNumIdx < 4) {
					secondNumBuff[secondNumIdx] = 0;
					secondNumIdx++;
					sprintf(str, "%d", 0);
					print(str);
				}
			}
			break;

		case 4:
			if (status == FIRST_NUM) {
				if (firstNumIdx == 0) {
					firstNumSign = NEGATIVE;
					setCursor(0, 0);
				} else {
					status = SECOND_NUM;
					operator = MINUS;
				}
				print("-");
			} else if (status == RESULT) {
				status = SECOND_NUM;
				operator = MINUS;
				print("-");
			}
			break;

		case 8:
			if (status == FIRST_NUM) {
				if (firstNumIdx == 0) {
					firstNumSign = POSITIVE;
				} else {
					status = SECOND_NUM;
					operator = PLUS;
					print("+");
				}
			} else if (status == RESULT) {
				status = SECOND_NUM;
				operator = PLUS;
				print("+");
			}
			break;

		case 12:
			if (status == FIRST_NUM) {
				status = SECOND_NUM;
				operator = MULTIPLY;
				print("*");
			} else if (status == RESULT) {
				status = SECOND_NUM;
				operator = MULTIPLY;
				print("*");
			}
			break;
		case 13:
			for (int i = 0; i < 4; i++)
				firstNumBuff[i] = secondNumBuff[i] = 0;
			firstNumIdx = secondNumIdx = 0;
			status = FIRST_NUM;
			firstNumSign = POSITIVE;
			currentRow = 0;
			clear();
			setCursor(0, 0);
			print("0");
			break;
		case 15:
			if (status == SECOND_NUM) {
				if (secondNumIdx > 0) {
					calculate();
				}
			}
			break;
		case 16:
			if (status == FIRST_NUM) {
				status = SECOND_NUM;
				operator = DIVISION;
				print("/");
			} else if (status == RESULT) {
				status = SECOND_NUM;
				operator = DIVISION;
				print("/");
			}
			break;

		default:
			calcChar = 'j';
			print("j");
			break;
		}

		clicked = false;

	}
}

GPIO_TypeDef *const Row_ports[] = { GPIOD, GPIOD, GPIOD, GPIOD };
const uint16_t Row_pins[] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3 };
GPIO_TypeDef *const Column_ports[] = { GPIOD, GPIOD, GPIOD, GPIOD };
const uint16_t Column_pins[] = { GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7 };
uint32_t last_gpio_exti;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (last_gpio_exti + 200 > HAL_GetTick()) {
		return;
	}
	last_gpio_exti = HAL_GetTick();


	int8_t row_number = -1;
	int8_t column_number = -1;

	for (uint8_t row = 0; row < 4; row++) // Loop through Rows
			{
		if (GPIO_Pin == Row_pins[row]) {
			row_number = row;
		}
	}

	for (int i = 0; i < 4; i++)
		HAL_GPIO_WritePin(Column_ports[i], Column_pins[i], GPIO_PIN_RESET);

	for (uint8_t col = 0; col < 4; col++) // Loop through Columns
			{
		HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
		if (HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number])) {
			column_number = col;
		}
		HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
	}

	for (int i = 0; i < 4; i++)
		HAL_GPIO_WritePin(Column_ports[i], Column_pins[i], GPIO_PIN_SET);

	if (row_number == -1 || column_number == -1) {
		return; // Reject invalid scan
	}
//   C0   C1   C2   C3
// +----+----+----+----+
// | 1  | 2  | 3  | 4  |  R0
// +----+----+----+----+
// | 5  | 6  | 7  | 8  |  R1
// +----+----+----+----+
// | 9  | 10 | 11 | 12 |  R2
// +----+----+----+----+
// | 13 | 14 | 15 | 16 |  R3
// +----+----+----+----+
	const uint8_t button_number = row_number * 4 + column_number + 1;
	myNum = button_number;
	clicked = true;
}

void calcPrint(const char *c) {
	print(c);
}

int result = 0;
void calculate() {
	status = RESULT;
	int firstNum = 0;
	switch (firstNumIdx) {
	case 4:
		firstNum += 1000 * firstNumBuff[0];
		firstNum += 100 * firstNumBuff[1];
		firstNum += 10 * firstNumBuff[2];
		firstNum += firstNumBuff[3];
		break;
	case 3:
		firstNum += 100 * firstNumBuff[0];
		firstNum += 10 * firstNumBuff[1];
		firstNum += firstNumBuff[2];
		break;
	case 2:
		firstNum += 10 * firstNumBuff[0];
		firstNum += firstNumBuff[1];
		break;
	case 1:
		firstNum += firstNumBuff[0];
		break;
	}

	int secondNum = 0;
	switch (secondNumIdx) {
	case 4:
		secondNum += 1000 * secondNumBuff[0];
		secondNum += 100 * secondNumBuff[1];
		secondNum += 10 * secondNumBuff[2];
		secondNum += secondNumBuff[3];
		break;
	case 3:
		secondNum += 100 * secondNumBuff[0];
		secondNum += 10 * secondNumBuff[1];
		secondNum += secondNumBuff[2];
		break;
	case 2:
		secondNum += 10 * secondNumBuff[0];
		secondNum += secondNumBuff[1];
		break;
	case 1:
		secondNum += secondNumBuff[0];
		break;
	}

	if (firstNumSign == NEGATIVE) {
		firstNum = -firstNum;
	}

	float fResult = 0;

	switch (operator) {
	case PLUS:
		result = firstNum + secondNum;
		break;
	case MINUS:
		result = firstNum - secondNum;
		break;
	case MULTIPLY:
		result = firstNum * secondNum;
		break;
	case DIVISION:
		if (secondNum == 0) {
			status = ERROR0;
			clear();
			print("DIVIDE BY ZERO!!");
			return;
			break;
		}
		fResult = (float) firstNum / secondNum;
		sprintf(str, "%.2f", fResult);
		print("=");
		if (++currentRow == 4) {
			currentRow = 0;
			clear();
		} else
			setCursor(0, currentRow);
		print(str);
		return;
		break;
	}
	int tmp = result;
	firstNumBuff[3] = tmp % 10;
	tmp /= 10;
	firstNumBuff[2] = tmp % 10;
	tmp /= 10;
	firstNumBuff[1] = tmp % 10;
	tmp /= 10;
	firstNumBuff[0] = tmp % 10;
	firstNumIdx = 4;
	for (int i = 0; i < 4; i++)
		secondNumBuff[i] = 0;
	secondNumIdx = 0;
	sprintf(str, "%d", result);
	print("=");
	if (++currentRow == 4) {
		currentRow = 0;
		clear();
	} else
		setCursor(0, currentRow);
	print(str);
}
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
