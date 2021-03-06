/*
 * ST47_Sim800.h
 *
 *  Created on: May 23, 2021
 *      Author: Vo Son Tung HUST - Night Owl
 *      Sim800L STM32 Library
 */

#ifndef INC_ST47_SIM800_H_
#define INC_ST47_SIM800_H_

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#define SIMCOM_RESPONSE_MAX_SIZE	100

extern UART_HandleTypeDef huart3;
typedef struct
{
	uint8_t response[SIMCOM_RESPONSE_MAX_SIZE];
	uint8_t index;
}AT_Command_Struct;

typedef struct
{
	bool post_status;
}Firebase_Struct;

typedef struct
{
	AT_Command_Struct	at_cmd;
	Firebase_Struct		firebase;
}Simcom_Struct;


/* ---------Simcom AT Function-------------*/
void simcom_delete_buffer(char* buffer);
uint8_t simcom_at_command(char* command, char* response, uint32_t timeout);
void simcom_init();
void simcom_enter_sleep_mode();

/*----------Simcom Message-----------------*/
void simcom_send_sms(char* phone_number, char* message);


/*----------Simcom GPRS Function-----------*/
void simcom_gprs_init();
void simcom_gprs_start();
void simcom_gprs_http_start();
void simcom_gprs_http_end();
void simcom_gprs_end();
void simcom_gprs_http_set_ssl();
bool simcom_server_post_status();

/*----------Simcom Firebase-------------*/
void firebase_update(char* url, char* device_id, char* user_id, float data1, float data2);
char* firebase_read_json();
bool firebase_post_status();
#endif /* INC_ST47_SIM800_H_ */
