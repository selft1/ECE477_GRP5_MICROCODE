/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "fingerprint_scanner.h"
#include "ir_sensor.h"
#include "raspi_coms.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

void SendtoPi(int select, char string[100], int input);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//== UART 2 STUFF ==//
char Rx_data2[100];
char buff[12];
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
char Rx_data[100];
char old_Rx_data[100];
int len;

unsigned char buffer[300];
//==================

//== ADC STUFF ==//
ADC_HandleTypeDef hadc;
int image_taken = 0;
int first_run = 0;
int ir_cnt = 0;
uint16_t adc_value;
//===============

//== UART STUFF ==//
//int len;
//char message[100];
int counter = 0;
//unsigned char buffer[100];
int check = 0;
bool returns = 0;

//== FLAGS ==//
bool no_ir_flag = false;

//== BUTTONS ==//
int btn_delay = 10;
bool reset_btn_last = false;
int reset_delay = 0;
bool reset_on = false;
bool newusr_btn_last = false;
int newusr_delay = 0;
bool newusr_on = false;
bool door_btn_last = false;
int door_delay = 0;
bool door_on = false;
bool key_btn_last = false;
int key_delay = 0;
bool key_on = false;
bool finger_last = false;
int finger_delay = 0;
bool finger_on = 0;


int timeout = 500000;
int timeout2 = 1000000;
int timeout_cnt = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // change this for recieve from both UARTs
{
	if(Rx_data[0] == old_Rx_data[0] && Rx_data[1] == old_Rx_data[1]){
		check = Check_Response(Rx_data2);
		//Rx_data2[0] = 0x0;
	}
	else{
		if(Rx_data[0] == 0x03){ // delete single user
			int del_id = 0;
			del_id = del_id + (int)Rx_data[1];
			//check database for user and let pi know if user doesn't exist. let pi know if completed successfully
			SendtoFP(Delete_packet(del_id));
		}
		else if(Rx_data[0] == 0x04){
			int new_id = Enroll_finger();
			  LED_FP(1);
			  no_ir_flag = true;
			  while(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) == 0 && timeout_cnt < timeout){timeout_cnt++;} // put finger on scanner
			  if(timeout_cnt < timeout){
				  int new_id = Enroll_finger();
				  SendNewUserFPS(new_id);
			  }
			  timeout_cnt = 0;
			  LED_FP(0);
			  while(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) != 0 && timeout_cnt < timeout2){timeout_cnt++;} // get finger off scanner
			  timeout_cnt = 0;
		}
		else if(Rx_data[0] == 0x02) { // delete all users
			SendtoFP(Deleteall_packet());
		}
		else if(Rx_data[0] == 0x01) { // unlock
			GetUnlockDoor();
		}
		old_Rx_data[0] = Rx_data[0];
		old_Rx_data[1] = Rx_data[1];
		//packet came in from rpi
	}

	HAL_UART_Receive_IT (&huart2, Rx_data2, 12);
	HAL_UART_Receive_IT (&huart1, Rx_data, 2);
}

void SendtoPi(int select, char string[100], int input){
	if(select == 1){
		sprintf(buffer,string);
		len = strlen(buffer);
	}else if(select == 0){
		sprintf(buffer,"%d",input);
		len = strlen(buffer);
	}
	HAL_UART_Transmit(&huart1,buffer,len,1000);
	sprintf(buffer,"\n\r");
	len = strlen(buffer);
	HAL_UART_Transmit(&huart1,buffer,len,1000);
}

void SendSnap(void){
	 char message[300];
	 snprintf(message,sizeof(message),"{\"MessageID\":\"SNAP\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"\",\"Method\":\"\",\"AccessGranted\":,\"ImageID\":\"\"}\n");
	 SendtoPi(1,message,0);
}

void SendAccessFPS(int id){
	 char message[300];
	 SendSnap();
	 snprintf(message,sizeof(message),"{\"MessageID\":\"NEW_RECORD\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"%d\",\"Method\":\"SCANNER\",\"AccessGranted\":true,\"ImageID\":\"\"}\n",id);
	 SendtoPi(1,message,0);
}

void SendDeniedFPS(void){
	 char message[300];
	 SendSnap();
	 snprintf(message,sizeof(message),"{\"MessageID\":\"NEW_RECORD\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"\",\"Method\":\"SCANNER\",\"AccessGranted\":false,\"ImageID\":\"\"}\n");
	 SendtoPi(1,message,0);
}

void SendDeniedIR(void){
	 char message[300];
	 SendSnap();
	 snprintf(message,sizeof(message),"{\"MessageID\":\"NEW_RECORD\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"\",\"Method\":\"\",\"AccessGranted\":false,\"ImageID\":\"\"}\n");
	 SendtoPi(1,message,0);
}

void SendNewUserFPS(int id){
	 char message[300];
	 SendSnap();
	 snprintf(message,sizeof(message),"{\"MessageID\":\"NEW_USER\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"%d\",\"Method\":\"\",\"AccessGranted\":,\"ImageID\":\"\"}\n",id);
	 SendtoPi(1,message,0);
}

void GetUnlockDoor(void){
	  OpenLatch();
	  HAL_Delay(10);
	  CloseLatch();
}

void SendAccessKey(){
	 char message[300];
	 SendSnap();
	 snprintf(message,sizeof(message),"{\"MessageID\":\"NEW_RECORD\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"\",\"Method\":\"KEY\",\"AccessGranted\":true,\"ImageID\":\"\"}\n");
	 SendtoPi(1,message,0);
}

void OpenLatch(){
	  HAL_GPIO_WritePin(GPIOC, SOLENOID_ENABLE_Pin, SET);
	  //HAL_Delay(5000);
	  HAL_GPIO_WritePin(GPIOC, SOLENOID_Pin, SET);
}

void CloseLatch(){
	  HAL_GPIO_WritePin(GPIOC, SOLENOID_Pin, RESET);
	  HAL_GPIO_WritePin(GPIOC, SOLENOID_ENABLE_Pin, RESET);
}

bool Debounce(int btn){
	bool return_val = false;
	switch(btn){
	case 0: // reset button
		if(HAL_GPIO_ReadPin(GPIOA, BCK_RESET_BTN_Pin) == 0 && reset_on == false){
			if(reset_btn_last == false){
				reset_btn_last = true;
				reset_delay = 0;
			}
			else {
				if(reset_delay == btn_delay){
					return_val = true;
					reset_delay = 0;
					reset_on = true;
				}
				else{
					reset_delay++;
				}
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOA, BCK_RESET_BTN_Pin) != 0){
			reset_btn_last = false;
			reset_on = false;
		}
		else if(reset_on == true){
			return_val = true;
		}
		break;
	case 1: // new user button
		if(HAL_GPIO_ReadPin(GPIOA, BCK_NEWUSER_BTN_Pin) == 0 && newusr_on == false){
			if(newusr_btn_last == false){
				newusr_btn_last = true;
				newusr_delay = 0;
			}
			else {
				if(newusr_delay == btn_delay){
					return_val = true;
					newusr_delay = 0;
					newusr_on = true;
				}
				else{
					newusr_delay++;
				}
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOA, BCK_NEWUSER_BTN_Pin) != 0){
			newusr_btn_last = false;
			newusr_on = false;
		}
		else if(newusr_on == true){
			return_val = true;
		}
		break;
	case 2: // key button
		if(HAL_GPIO_ReadPin(GPIOC, BTM_KEYUSED_BTN_Pin) == 0 && key_on == false){
			if(key_btn_last == false){
				key_btn_last = true;
				key_delay = 0;
			}
			else {
				if(key_delay == btn_delay){
					return_val = true;
					key_delay = 0;
					key_on = true;
				}
				else{
					key_delay++;
				}
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOC, BTM_KEYUSED_BTN_Pin) != 0){
			key_btn_last = false;
			key_on = false;
		}
		else if(key_on == true){
			return_val = true;
		}
		break;
	case 3: // door button
		if(HAL_GPIO_ReadPin(GPIOC, SIDE_DOOROPEN_BTN_Pin) != 0 && door_on == false){
			if(door_btn_last == false){
				door_btn_last = true;
				door_delay = 0;
			}
			else {
				if(door_delay == btn_delay){
					return_val = true;
					door_delay = 0;
					door_on = true;
				}
				else{
					door_delay++;
				}
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOC, SIDE_DOOROPEN_BTN_Pin) == 0){
			door_btn_last = false;
			door_on = false;
		}
		else if(door_on == true){
			return_val = true;
		}
		break;
	case 4: // fingerprint scanner detect
		if(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) != 0 && finger_on == false){
			if(finger_last == false){
				finger_last = true;
				finger_delay = 0;
			}
			else {
				if(finger_delay == btn_delay){
					return_val = true;
					finger_delay = 0;
					finger_on = true;
				}
				else{
					finger_delay++;
				}
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) == 0){
			finger_last = false;
			finger_on = false;
		}
		else if(finger_on == true){
			return_val = true;
		}
		break;
	}

	return return_val;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
 adc_value = HAL_ADC_GetValue(hadc1);
 if(no_ir_flag == false && adc_value > 3250 && image_taken == 0 && first_run == 2){
	 ir_cnt++;

	 //if(ir_cnt > 1000000){
	 if(ir_cnt > 100000){
		 SendDeniedIR();
		 ir_cnt = 0;
		 image_taken = 1;
	 }
	 //ir_flag = 1;
 }
 else if(adc_value <= 3200 || no_ir_flag == true){
	 image_taken = 0;
	 ir_cnt = 0;
	 no_ir_flag = false;
	 //ir_flag = 0;
 }
 if(first_run < 2){first_run++;}

 HAL_ADC_Start_IT(hadc1);
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_IT(&hadc);
  HAL_GPIO_WritePin(GPIOC, SOLENOID_ENABLE_Pin, RESET);
  HAL_GPIO_WritePin(GPIOA, ELECTRO_ENABLE_Pin, RESET);
  HAL_GPIO_WritePin(GPIOC, ELECTROMAGNET_Pin, RESET);
  HAL_GPIO_WritePin(GPIOC, SOLENOID_Pin, RESET);
  HAL_GPIO_WritePin(GPIOB, SOLENOID_INVERT_Pin, RESET);
  HAL_GPIO_WritePin(GPIOB, ELECTROMAGNET_INVERT_Pin, RESET);

  HAL_GPIO_WritePin(GPIOA, IR_RECIEVE_Pin, SET);

  LED_FP(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(Debounce(4)){ // IDENTIFY FINGER
		  no_ir_flag = true;
		  LED_FP(1);
		  int db_check;
		  if((db_check = Check_DB()) == 1){
			  int id = Identify_finger();
			  if(id == -1 || id == 0) {// finger not found
				  SendDeniedFPS();
			  }
			  else{ // access granted
				  //for(int i = 0; i < id; i++){
					//  Flash_LED();
				  //}
				  SendAccessFPS(id);
			  }
		  }
		  else{SendDeniedFPS();}
		  //LED_FP(0);
		  while(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) != 0 && timeout_cnt < timeout2){timeout_cnt++;} // get finger off scanner
		  timeout_cnt = 0;
	  }
	  else if(Debounce(0)){ //RESET
		  no_ir_flag = true;
		  LED_FP(1);
		  HAL_Delay(1000);
		  LED_FP(0);
		  NVIC_SystemReset();
	  }
	  else if(Debounce(2)){
		  no_ir_flag = true;
		  SendAccessKey();
	  }
	  //else if(Debounce(3)){
		//  no_ir_flag = true;
	  //}
	  else if(Debounce(1)){ //MANUAL LEARN FINGER
		  LED_FP(1);
		  no_ir_flag = true;
		  while(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) == 0 && timeout_cnt < timeout){timeout_cnt++;} // put finger on scanner
		  if(timeout_cnt < timeout){
			  int new_id = Enroll_finger();
			  SendNewUserFPS(new_id);
		  }
		  timeout_cnt = 0;
		  LED_FP(0);
		  while(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) != 0 && timeout_cnt < timeout2){timeout_cnt++;} // get finger off scanner
		  timeout_cnt = 0;
	  }
	  else{ no_ir_flag = false; }
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
