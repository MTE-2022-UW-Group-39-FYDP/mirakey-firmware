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
#include "usb_device.h"
#include "key_config.h"
#include "mirakey_send_hid.h"

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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t active_layer;
char layer_chars[NUM_LAYERS][11] = LAYER_LAYOUTS;
uint8_t debounced;
int count;
uint8_t key_pressed_in_loop;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void ToggleLayer() {
	active_layer = (active_layer + 1) % NUM_LAYERS;
	/* UPDATE DISPLAYS */
//	for (int key = 0; key < 12; key++) {
//		MKS_TxGlyph(0b11111101+key, mapCharToBitmap(layer_chars[active_layer][key]));	// I'm assuming here that slave address numbers are consecutive
//	}
}

void debounceAndToggleLayer() {
	debounced = 0;
	count = 0;
	while (!debounced) {
		if (LAYER_KEY == KEY_DOWN) {
			count++;
			if (count >= 5) {
				debounced = 1;
			}
		} else {
			count = 0;
		}
	}

	ToggleLayer();

	debounced = 0;
	count = 0;
	while (!debounced) {
		if (LAYER_KEY == KEY_UP) {
			count++;
			if (count >= 5) {
				debounced = 1;
			}
		} else {
			count = 0;
		}
	}
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
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  active_layer = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if (LAYER_KEY == KEY_DOWN) {
		  debounceAndToggleLayer();
	  }

	  key_pressed_in_loop = 0;
	  if (K1 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][0]);
		  key_pressed_in_loop = 1;
	  }

	  if (K2 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][1]);
		  key_pressed_in_loop = 1;
	  }

	  if (K3 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][2]);
		  key_pressed_in_loop = 1;
	  }

	  if (K4 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][3]);
		  key_pressed_in_loop = 1;
	  }

	  if (K5 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][4]);
		  key_pressed_in_loop = 1;
	  }

	  if (K6 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][5]);
		  key_pressed_in_loop = 1;
	  }

	  if (K7 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][6]);
		  key_pressed_in_loop = 1;
	  }

	  if (K8 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][7]);
		  key_pressed_in_loop = 1;
	  }

	  if (K9 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][8]);
		  key_pressed_in_loop = 1;
	  }

	  if (K10 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][9]);
		  key_pressed_in_loop = 1;
	  }

	  if (K11 == KEY_DOWN) {
		  hid_send_char(layer_chars[active_layer][10]);
		  key_pressed_in_loop = 1;
	  }

	  if (!key_pressed_in_loop) {
		  num_times_recent_char_sent = 0;
	  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : K4_Pin K8_Pin K12_Pin K9_Pin
                           K5_Pin */
  GPIO_InitStruct.Pin = K4_Pin|K8_Pin|K12_Pin|K9_Pin
                          |K5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : K3_Pin K7_Pin K11_Pin K10_Pin
                           K6_Pin K2_Pin */
  GPIO_InitStruct.Pin = K3_Pin|K7_Pin|K11_Pin|K10_Pin
                          |K6_Pin|K2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : K1_Pin */
  GPIO_InitStruct.Pin = K1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(K1_GPIO_Port, &GPIO_InitStruct);

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
