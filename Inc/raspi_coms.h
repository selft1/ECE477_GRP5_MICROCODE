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
//#include "fingerprint_scanner.h"
/* USER CODE END Includes */



/* USER CODE BEGIN Private defines */
 int temp = 0;
 unsigned char buffer[300];
 int len;
/* USER CODE END Private defines */



/* USER CODE BEGIN Prototypes */

void SendtoPi(int select, char string[100], int input){
	if(select == 1){
		sprintf(buffer,string);
		len = strlen(buffer);
	}else if(select == 0){
		sprintf(buffer,"%d",input);
		len = strlen(buffer);
	}
	HAL_UART_Transmit(&huart1,buffer,len,1000);
	HAL_Delay(10);
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
	  WaitLatch(5000);
}

void SendAccessKey(){
	 char message[300];
	 SendSnap();
	 snprintf(message,sizeof(message),"{\"MessageID\":\"NEW_RECORD\",\"DoorID\":\"\",\"Date\":\"\",\"Time\":\"\",\"UserID\":\"\",\"Method\":\"KEY\",\"AccessGranted\":true,\"ImageID\":\"\"}\n");
	 SendtoPi(1,message,0);
}

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
