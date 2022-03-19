/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

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
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOC
#define K4_Pin GPIO_PIN_1
#define K4_GPIO_Port GPIOC
#define K8_Pin GPIO_PIN_2
#define K8_GPIO_Port GPIOC
#define K12_Pin GPIO_PIN_3
#define K12_GPIO_Port GPIOC
#define K3_Pin GPIO_PIN_0
#define K3_GPIO_Port GPIOA
#define K7_Pin GPIO_PIN_1
#define K7_GPIO_Port GPIOA
#define K11_Pin GPIO_PIN_2
#define K11_GPIO_Port GPIOA
#define K10_Pin GPIO_PIN_3
#define K10_GPIO_Port GPIOA
#define K6_Pin GPIO_PIN_4
#define K6_GPIO_Port GPIOA
#define K2_Pin GPIO_PIN_6
#define K2_GPIO_Port GPIOA
#define K9_Pin GPIO_PIN_4
#define K9_GPIO_Port GPIOC
#define K5_Pin GPIO_PIN_5
#define K5_GPIO_Port GPIOC
#define K1_Pin GPIO_PIN_0
#define K1_GPIO_Port GPIOB
#define MKS_CS0_Pin GPIO_PIN_2
#define MKS_CS0_GPIO_Port GPIOB
#define MKS_CS1_Pin GPIO_PIN_13
#define MKS_CS1_GPIO_Port GPIOB
#define MKS_CS4_Pin GPIO_PIN_15
#define MKS_CS4_GPIO_Port GPIOB
#define MKS_CS5_Pin GPIO_PIN_7
#define MKS_CS5_GPIO_Port GPIOC
#define MKS_CS6_Pin GPIO_PIN_9
#define MKS_CS6_GPIO_Port GPIOA
#define MKS_CS2_Pin GPIO_PIN_10
#define MKS_CS2_GPIO_Port GPIOC
#define MKS_CS3_Pin GPIO_PIN_12
#define MKS_CS3_GPIO_Port GPIOC
#define MKS_CS7_Pin GPIO_PIN_4
#define MKS_CS7_GPIO_Port GPIOB
#define MKS_DC_Pin GPIO_PIN_6
#define MKS_DC_GPIO_Port GPIOB
#define MKS_RES_Pin GPIO_PIN_7
#define MKS_RES_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
