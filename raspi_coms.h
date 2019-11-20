/**
  ******************************************************************************
  * File Name          : raspi_coms.h
  * Description        : This file includes code for the function of the
  * 						raspberry pi communication.
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
#ifndef __raspi_coms_H
#define __raspi_coms_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */



/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */



/* USER CODE BEGIN Prototypes */



/*
 void SendJSON_Test(int count){
 	 char message[100];
	 strcpy(message,"");

 	if(count == 0){
 		strcpy(message,"{\"MessageID\":\"NEW_RECORD\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"11\",\"Method\":\"Key\",\"AccessGranted\":\"FALSE\",\"ImageID\":\"\"}\n"); //NEW RECORD
 		}
 	if(count == 1){
 		strcpy(message,"{\"MessageID\":\"SNAP\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"\",\"Method\":\"\",\"AccessGranted\":\"\",\"ImageID\":\"\"}\n"); 				  //SNAP
 	}
 	if(count == 2){
 		strcpy(message,"{\"MessageID\":\"Granted\",\"UserID\":\"211\",\"Method\":\"Finger\"}\n"); // Granted: Finger
 	}
 	if(count == 3){
 		strcpy(message,"{\"MessageID\":\"Granted\",\"UserID\":\"\",\"Method\":\"Key\"}\n"); // Granted: Key
 	}
 	if(count == 4){
 		strcpy(message,"{\"MessageID\":\"Denied\",\"UserID\":\"\",\"Method\":\"Finger\"}\n"); // Denied
 	}
 	SendtoPi(1,message,0);
 }
 */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ raspi_coms_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
