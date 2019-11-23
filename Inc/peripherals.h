/**
  ******************************************************************************
  * File Name          : peripherals.h
  * Description        : This file provides code for the configuration
  *                      of the other peripherals on the pcb.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __peripherals_H
#define __peripherals_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */



/* USER CODE BEGIN Private defines */
 int temp1 = 0;
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
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
 void OpenLatch(){
 	  HAL_GPIO_WritePin(GPIOC, SOLENOID_ENABLE_Pin, SET);
 	  //HAL_Delay(5000);
 	  HAL_GPIO_WritePin(GPIOC, SOLENOID_Pin, SET);
 }

 void CloseLatch(){
 	  HAL_GPIO_WritePin(GPIOC, SOLENOID_Pin, RESET);
 	  HAL_GPIO_WritePin(GPIOC, SOLENOID_ENABLE_Pin, RESET);
 }

 bool WaitFingerPlace(int timeoutplace){
	 int timeout_temp = 0;
	 while(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) == 0 && timeout_temp < timeoutplace){timeout_temp++;}
	 if(timeout_temp < timeoutplace){
		 return true;
	 }
	 return false;
 }

 void WaitFingerRemove(int timeoutremove){
	 int timeout_temp = 0;
	 while(HAL_GPIO_ReadPin(GPIOC, FINGERPRINT_SENSE_Pin) != 0 && timeout_temp < timeoutremove){timeout_temp++;}
	 if(timeout_temp < timeoutremove){
		 return true;
	 }
	 return false;
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

 void WaitLatch(int holdtimer){
	 int timer_temp = 0;
	 while(timer_temp < holdtimer){
		 if(Debounce(3)){
			 CloseLatch();
			 break;
		 }
		 timer_temp++;
	 }
 }

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ peripherals_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
