/**
  ******************************************************************************
  * File Name          : fingerprint_scanner.h
  * Description        : This file contains all the functions prototypes for
  *                      the finger print scanner
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fingerptin_scanner_H
#define __fingerprint_scanner_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include <stdio.h>

//========== DEFINES ==========
#define DEVICE_ID_1 0x00;
#define DEVICE_ID_2 0x01;
#define CMD_OPEN 0x01;
#define CMD_CLOSE 0x02;
#define CMD_CMOSLED 0x12;
#define CMD_ENROLLCNT 0x20;
#define CMD_CHKENROLLED 0x21;
#define CMD_START_ENROLL 0x22;
#define CMD_ENROLL1 0x23;
#define CMD_ENROLL2 0x24;
#define CMD_ENROLL3 0x25;
#define CMD_ISFINGER 0x26;
#define CMD_DEL_ID 0x40;
#define CMD_DEL_ALL 0x41;
#define CMD_VERIFY 0x50;
#define CMD_IDENTIFY 0x51;
#define CMD_STANDBY 0xF9;
#define CMD_ACK 0x30;
#define CMD_NACK 0x31;
#define CMD_CAPTURE 0x60;
#define ZERO 0x00000000;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long double DATA;
extern int check;
extern char Rx_data2[100];
char responseParam[4];
//=============================

//========== PACKETS ==========

// COMMAND/RESPONSE PACKET
typedef struct packet {
	BYTE start_code1;// = 0x55;
	BYTE start_code2;// = 0xAA;
	WORD device_id;// = 0x0001;
	DWORD parameter;
	WORD command;
	WORD checksum;
}packet;

// DATA PACKET
typedef struct data_packet {
	BYTE data_start_code1;
	BYTE data_start_code2;
	WORD data_device_id;
	DATA data;
	WORD data_checksum;
}data_packet;
//=============================

//========== FUNCTIONS ==========
struct packet Init_packet(){
	packet Pack;
	Pack.start_code1 = 0x55;
	Pack.start_code2 = 0xAA;
	Pack.device_id = 0x0001;
	return Pack;
}
int Check_Response(char response[12]){
	responseParam[0] = 0;
	responseParam[1] = 0;
	responseParam[2] = 0;
	responseParam[3] = 0;
	if(response[0] == 0x55 && response[1] == 0xAA && response[8] == 0x30){//ACK
	//if((response >> 8 == 0x55AA0100) && ((response & 0xFFFFFFFF) == 0x30003001)){//ACK
		responseParam[0] = response[4]; // save these values for checking
		responseParam[1] = response[5];
		responseParam[2] = response[6];
		responseParam[3] = response[7];
		return 1;
	} else if(response[8] == 0x31){
		return -1;
	} else { // ERROR
		return 0;
	}
}
bool Wait_Response(void){
	int timer = 0;
	while(check == false && timer < 210){
		HAL_Delay(10);
		timer++;
	}
	check = false;
	if(timer >= 200){ return 0; }
	else { return 1; }
	return 0;
}
WORD calc_Checksum(struct packet pkt){
	WORD chksm = 0;
	chksm += 0x55;
	chksm += 0xAA;
	chksm += DEVICE_ID_1; // first byte
	chksm += DEVICE_ID_2; // second byte
	chksm += (pkt.parameter) & 0xFF; //first byte
	chksm += (pkt.parameter >> 8) & 0xFF; // second byte
	chksm += (pkt.parameter >> 16) & 0xFF; // third byte
	chksm += (pkt.parameter >> 24) & 0xFF; // fourth byte
	chksm += (pkt.command) & 0xFF; //first byte
	chksm += (pkt.command >> 8) & 0xFF; //second byte
	return chksm;
}
void SendtoFP(struct packet pkt){
	char output[20];
	output[0] = pkt.start_code1; //point the beginning of output to the beginning of packet
	output[1] = pkt.start_code2;
	output[2] = pkt.device_id & 0xFF;
	output[3] = (pkt.device_id >> 8) & 0xFF;
	output[4] = (pkt.parameter) & 0xFF; //first byte
	output[5] = (pkt.parameter >> 8) & 0xFF; // second byte
	output[6] = (pkt.parameter >> 16) & 0xFF; // third byte
	output[7] = (pkt.parameter >> 24) & 0xFF; // fourth byte
	output[8] = (pkt.command) & 0xFF; //first byte
	output[9] = (pkt.command >> 8);// & 0xFF; //second byte
	output[10] = (pkt.checksum) & 0xFF; //first byte
	output[11] = (pkt.checksum >> 8);// & 0xFF; //second byte
	HAL_UART_Receive_IT (&huart2, Rx_data2, 12);
	HAL_UART_Transmit(&huart2,(uint8_t *)output,sizeof(output),1000);
}

struct packet Open_packet (void){
	packet open_packet = Init_packet();
	open_packet.parameter = ZERO;
	open_packet.command = CMD_OPEN;
	open_packet.checksum = calc_Checksum(open_packet);
	return(open_packet);
}
struct packet Close_packet (void){
	packet close_packet = Init_packet();
	close_packet.parameter = ZERO;
	close_packet.command = CMD_CLOSE;
	close_packet.checksum = calc_Checksum(close_packet);
	return(close_packet);
}
struct packet CMOSLED_packet (bool LED){
	packet cmosled_packet = Init_packet();
	if(LED){
		cmosled_packet.parameter = 0x1; // TURN ON LED
	}else{
		cmosled_packet.parameter = 0x0; // TURN OFF LED
	}
	cmosled_packet.command = CMD_CMOSLED;
	cmosled_packet.checksum = calc_Checksum(cmosled_packet);
	return(cmosled_packet);
}
struct packet Getenrolled_packet (void){
	packet getenrolled_packet = Init_packet();
	getenrolled_packet.parameter = ZERO;
	getenrolled_packet.command = CMD_ENROLLCNT;
	getenrolled_packet.checksum = calc_Checksum(getenrolled_packet);
	return(getenrolled_packet);
}
struct packet Checkenrll_packet (int ID){
	packet checkenrll_packet = Init_packet();
	checkenrll_packet.parameter = (char)ID;
	checkenrll_packet.command = CMD_CHKENROLLED;
	checkenrll_packet.checksum = calc_Checksum(checkenrll_packet);
	return(checkenrll_packet);
}
struct packet Enroll_packet (int ID){
	packet enroll_packet = Init_packet();
	enroll_packet.parameter = (char)ID;
	enroll_packet.command = CMD_START_ENROLL;
	enroll_packet.checksum = calc_Checksum(enroll_packet);
	return(enroll_packet);
}
struct packet Enroll1_packet (void){
	packet enroll1_packet = Init_packet();
	enroll1_packet.parameter = ZERO;
	enroll1_packet.command = CMD_ENROLL1;
	enroll1_packet.checksum = calc_Checksum(enroll1_packet);
	return(enroll1_packet);
}
struct packet Enroll2_packet (void){
	packet enroll2_packet = Init_packet();
	enroll2_packet.parameter = ZERO;
	enroll2_packet.command = CMD_ENROLL2;
	enroll2_packet.checksum = calc_Checksum(enroll2_packet);
	return(enroll2_packet);
}
struct packet Enroll3_packet (void){
	packet enroll3_packet = Init_packet();
	enroll3_packet.parameter = ZERO;
	enroll3_packet.command = CMD_ENROLL3;
	enroll3_packet.checksum = calc_Checksum(enroll3_packet);
	return(enroll3_packet);
}
struct packet Fingerpress_packet (void){
	packet fingerpress_packet = Init_packet();
	fingerpress_packet.parameter = ZERO;
	fingerpress_packet.command = CMD_ISFINGER;
	fingerpress_packet.checksum = calc_Checksum(fingerpress_packet);
	return(fingerpress_packet);
}
struct packet Delete_packet (int ID){
	packet delete_packet = Init_packet();
	delete_packet.parameter = ID;
	delete_packet.command = CMD_DEL_ID;
	delete_packet.checksum = calc_Checksum(delete_packet);
	return(delete_packet);
}
struct packet Deleteall_packet (void){
	packet deleteall_packet = Init_packet();
	deleteall_packet.parameter = ZERO;
	deleteall_packet.command = CMD_DEL_ALL;
	deleteall_packet.checksum = calc_Checksum(deleteall_packet);
	return(deleteall_packet);
}
struct packet Verify_packet (int ID){
	packet verify_packet = Init_packet();
	verify_packet.parameter = ID;
	verify_packet.command = CMD_VERIFY;
	verify_packet.checksum = calc_Checksum(verify_packet);
	return(verify_packet);
}
struct packet Ident_packet (void){
	packet ident_packet = Init_packet();
	ident_packet.parameter = ZERO;
	ident_packet.command = CMD_IDENTIFY;
	ident_packet.checksum = calc_Checksum(ident_packet);
	return(ident_packet);
}
struct packet Standby_packet (void){
	packet standby_packet = Init_packet();
	standby_packet.parameter = ZERO;
	standby_packet.command = CMD_STANDBY;
	standby_packet.checksum = calc_Checksum(standby_packet);
	return(standby_packet);
}
struct packet Capture_packet (bool QUALITY){
	packet capture_packet = Init_packet();
	if(QUALITY){
		capture_packet.parameter = 0x00000001;
	}else{
		capture_packet.parameter = ZERO;
	}
	capture_packet.command = CMD_CAPTURE;
	capture_packet.checksum = calc_Checksum(capture_packet);
	return(capture_packet);
}

//START COMMUNICATION
bool Start_FP(void){
	int timeout_counter = 0;
	//packet cmdpkt = Open_packet();
	SendtoFP(Open_packet());
	while(check == 0 && timeout_counter < 200){
		HAL_Delay(10);
		timeout_counter++;
	}
	if(timeout_counter < 200){
		return 1;
	}
	check = 0; // RESET CHECK FLAG FOR NEXT TRANSMISSION NO MATTER WHAT
	return 0;
}

//STOP COMMUNICATION SAFELY
bool Close_FP(void){
	int timeout_counter = 0;
	SendtoFP(Close_packet());
	while(check == 0 && timeout_counter < 200){
		HAL_Delay(10);
		timeout_counter++;
	}
	if(timeout_counter < 200){
		return 1;
	}
	check = 0; // RESET CHECK FLAG FOR NEXT TRANSMISSION NO MATTER WHAT
	return 0;
}

//TURN ON LED
bool LED_FP(int control){
	int timeout_counter;
	timeout_counter = 0;

	if(control == 1){ SendtoFP(CMOSLED_packet(1)); }
	else{ SendtoFP(CMOSLED_packet(0)); }

	//if(!(Wait_Response)){ return 0; }
	//return 1;
	while(check == 0 && timeout_counter < 5000){
		HAL_Delay(10);
		timeout_counter++;
	}
	//if(timeout_counter < 200){
	//	return 1;
	//}
	check = 0; // RESET CHECK FLAG FOR NEXT TRANSMISSION NO MATTER WHAT
	return 0;
}

//FLASH LED
void Flash_LED(void){
	LED_FP(0);
	HAL_Delay(100);
	LED_FP(1);
}

bool IsFingerPress(void){
	int timeout_counter = 0;
	SendtoFP(Fingerpress_packet());
	if(!(Wait_Response())){ return false; } // if we get an error code back, just say there isn't a finger pressed
	if(responseParam[0] == 0x0 && responseParam[1] == 0x0 && responseParam[2] == 0x0 && responseParam[3] == 0x0){ // finger is pressed
		return false;
	}else{ // finger is not pressed
		return true;
	}
}

//ENROL FINGER
int Enroll_finger (void){

	//DETERMINE VALID ID
	int id = 3;
	while(id < 200){
		SendtoFP(Checkenrll_packet(id));
		while(check == 0);
		if(check == -1){
			check = 0;
			break;
		}
		check = 0;
		//else if(check == 1){
			//return -1;
		//}
		id++;
	}
	if (id == 200){ return -2;} //return that we need to delete people
	LED_FP(1);
	//ENROLL START
	SendtoFP(Enroll_packet(id));
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	else if(check == -1){return -1;}

	//while(IsFingerPress()); //wait for finger to be placed

	//CAPTURE FINGER
	SendtoFP(Capture_packet(1));
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	else if(check == -1){return -1;}

	//ENROLL1
	SendtoFP(Enroll1_packet());
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	else if(check == -1){return -1;}

	//WAIT FOR FINGER TO BE TAKEN OFF
	//Flash_LED();
	//while(!IsFingerPress()); //wait for finger to be lifted off
	//HAL_Delay(1000);
	//while(!HAL_GPIO_ReadPin(GPIOA, BCK_RESET_BTN_Pin)); // TEMP USING BUTTON

	//CAPTURE FINGER
	SendtoFP(Capture_packet(1));
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	else if(check == -1){return -1;}

	//ENROLL2
	SendtoFP(Enroll2_packet());
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	else if(check == -1){return -1;}

	//WAIT FOR FINGER TO BE TAKEN OFF
	//Flash_LED();
	//HAL_Delay(1000);
	//while(!HAL_GPIO_ReadPin(GPIOA, BCK_RESET_BTN_Pin));

	//CAPTURE FINGER
	SendtoFP(Capture_packet(1));
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	else if(check == -1){return -1;}

	//ENROLL3
	SendtoFP(Enroll3_packet());
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	else if(check == -1){return -1;}

	Flash_LED();
	LED_FP(0);
	return(id);
}

int Identify_finger (void){
	int id = 0;
	LED_FP(1);
	SendtoFP(Capture_packet(1));
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	LED_FP(0);
	SendtoFP(Ident_packet());
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	//Convert hex response to int
	id = id + (int)responseParam[0];
	id = id + ((int)responseParam[1] * 16);
	id = id + ((int)responseParam[2] * 24);
	id = id + ((int)responseParam[3] * 32);

	if(id > 0){
		for(int i = 0; i < id; i++){
			Flash_LED();
		}
	}
	HAL_GPIO_WritePin(GPIOA, BCK_RED_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, BCK_GRN_LED_Pin, GPIO_PIN_RESET);
	return id;
}

int Check_DB (void){
	int ids = 0;
	SendtoFP(Getenrolled_packet());
	while(check == 0);	//check for ACK
	if(check == 1){
		check = 0;
	}
	//Convert hex response to int
	ids = ids + (int)responseParam[0];
	ids = ids + ((int)responseParam[1] * 16);
	ids = ids + ((int)responseParam[2] * 24);
	ids = ids + ((int)responseParam[3] * 32);
	if(ids > 0){
		//for (int i = 0; i < ids; i++){
			//Flash_LED();
		//}
		return 1;
	}
	return 0;
}

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE****/
