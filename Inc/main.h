/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#define SOLENOID_ENABLE_Pin GPIO_PIN_13
#define SOLENOID_ENABLE_GPIO_Port GPIOC
#define SIDE_DOOROPEN_BTN_Pin GPIO_PIN_1
#define SIDE_DOOROPEN_BTN_GPIO_Port GPIOC
#define BTM_KEYUSED_BTN_Pin GPIO_PIN_2
#define BTM_KEYUSED_BTN_GPIO_Port GPIOC
#define BCK_RESET_BTN_Pin GPIO_PIN_0
#define BCK_RESET_BTN_GPIO_Port GPIOA
#define BCK_NEWUSER_BTN_Pin GPIO_PIN_1
#define BCK_NEWUSER_BTN_GPIO_Port GPIOA
#define UART_FINGER_TX_Pin GPIO_PIN_2
#define UART_FINGER_TX_GPIO_Port GPIOA
#define UART_FINGER_RX_Pin GPIO_PIN_3
#define UART_FINGER_RX_GPIO_Port GPIOA
#define ELECTRO_ENABLE_Pin GPIO_PIN_6
#define ELECTRO_ENABLE_GPIO_Port GPIOA
#define IR_SEND_Pin GPIO_PIN_5
#define IR_SEND_GPIO_Port GPIOC
#define ELECTROMAGNET_INVERT_Pin GPIO_PIN_14
#define ELECTROMAGNET_INVERT_GPIO_Port GPIOB
#define SOLENOID_INVERT_Pin GPIO_PIN_15
#define SOLENOID_INVERT_GPIO_Port GPIOB
#define ELECTROMAGNET_Pin GPIO_PIN_6
#define ELECTROMAGNET_GPIO_Port GPIOC
#define SOLENOID_Pin GPIO_PIN_7
#define SOLENOID_GPIO_Port GPIOC
#define BRD_BLUE_LED_Pin GPIO_PIN_8
#define BRD_BLUE_LED_GPIO_Port GPIOC
#define FINGERPRINT_SENSE_Pin GPIO_PIN_9
#define FINGERPRINT_SENSE_GPIO_Port GPIOC
#define IR_RECIEVE_Pin GPIO_PIN_8
#define IR_RECIEVE_GPIO_Port GPIOA
#define UART_RPI_TX_Pin GPIO_PIN_9
#define UART_RPI_TX_GPIO_Port GPIOA
#define UART_RPI_RX_Pin GPIO_PIN_10
#define UART_RPI_RX_GPIO_Port GPIOA
#define BCK_GRN_LED_Pin GPIO_PIN_11
#define BCK_GRN_LED_GPIO_Port GPIOA
#define BCK_RED_LED_Pin GPIO_PIN_12
#define BCK_RED_LED_GPIO_Port GPIOA
#define FRT_GRN_LED_Pin GPIO_PIN_3
#define FRT_GRN_LED_GPIO_Port GPIOB
#define FRT_RED_LED_Pin GPIO_PIN_4
#define FRT_RED_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
