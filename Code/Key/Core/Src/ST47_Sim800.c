/*
 * ST47_Sim800.c
 *
 *  Created on: May 23, 2021
 *      Author: Vo Son Tung HUST - Night Owl
 */

#include "ST47_Sim800.h"
#include "Float_String_Convert.h"
#include "stdbool.h"
Simcom_Struct simcom;
char json_test[100];

extern uint8_t flag;
void simcom_delete_buffer(char* buffer)
{
	simcom.at_cmd.index = 0;
	for(int i=0; i<SIMCOM_RESPONSE_MAX_SIZE;i++)
	{
		buffer[i] = 0;
	}
}

uint8_t simcom_at_command(char* command, char* response, uint32_t timeout)
{
	uint8_t status = 0;
	uint8_t received_byte[1];
	simcom_delete_buffer((char*)simcom.at_cmd.response);
	uint32_t time_out_transmit = HAL_GetTick();
	uint32_t time_out_get_response = HAL_GetTick();

	HAL_UART_Transmit(&huart3, (uint8_t*)command, strlen(command), 1000);
	HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n", strlen("\r\n"), 1000);

	while(status == 0)
	{
		while(HAL_UART_Receive(&huart3, received_byte, 1, 1000) != HAL_OK)
		{
			if(HAL_GetTick() - time_out_transmit > timeout)
			{
				return 0;
			}
		}
		time_out_transmit = HAL_GetTick();
		simcom.at_cmd.response[simcom.at_cmd.index++] = received_byte[0];
		while(HAL_GetTick() - time_out_transmit < timeout)
		{
			if(HAL_UART_Receive(&huart3, received_byte, 1, 1000) == HAL_OK)
			{
				simcom.at_cmd.response[simcom.at_cmd.index++] = received_byte[0];
				time_out_get_response = HAL_GetTick();
			}
			else
			{
				if(HAL_GetTick() - time_out_get_response > 100)
				{
					if(strstr((char*)simcom.at_cmd.response,response) != NULL)
					{
						status = 1;
					}
					else
					{
						status = 0;
					}
					break;
				}
			}
		}
	}
	return status;
}

void simcom_init()
{
	simcom_at_command("AT", "OK", 1000);
	HAL_Delay(100);
	simcom_at_command("ATE0", "OK", 1000);
	HAL_Delay(100);
	simcom_gprs_http_end();
	simcom_gprs_end();
	simcom_gprs_start();
	simcom_gprs_http_start();
}

void simcom_gprs_start()
{
	simcom_at_command("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 3000);
	HAL_Delay(100);

	simcom_at_command("AT+SAPBR=3,1,\"APN\",\"v-internet\"", "OK", 3000);
	HAL_Delay(100);

	simcom_at_command("AT+SAPBR=1,1", "OK", 3000);
	HAL_Delay(100);

	simcom_at_command("AT+SAPBR=2,1", "+SAPBR:", 3000);
	HAL_Delay(100);
}

void simcom_gprs_http_start()
{
	simcom_at_command("AT+HTTPINIT", "OK", 3000);
	HAL_Delay(100);

	simcom_at_command("AT+HTTPPARA=\"CID\",1", "OK", 3000);
	HAL_Delay(100);
}

void simcom_gprs_http_set_ssl()
{
	simcom_at_command("AT+HTTPSSL=1", "OK", 1000);
	HAL_Delay(100);
}

void simcom_gprs_http_end()
{
	simcom_at_command("AT+HTTPTERM", "OK", 3000);
	HAL_Delay(100);
}

void simcom_gprs_end()
{
	simcom_at_command("AT+SAPBR=0,1", "OK", 3000);
	HAL_Delay(100);
}

void firebase_update(float data1, float data2)
{
	simcom_at_command("AT+HTTPPARA=\"URL\",\"https://key-gps-tracking-default-rtdb.firebaseio.com/id.json?x-http-method-override=PATCH\"", "OK", 1000);
	HAL_Delay(500);

	simcom_at_command("AT+HTTPPARA=\"CONTENT\",\"application/json\"", "OK", 1000);
	HAL_Delay(500);

	simcom_at_command("AT+HTTPDATA=100,25000", "DOWNLOAD", 1000);
	HAL_Delay(500);

	char* json = malloc(80);
	char lat[10],lng[10];
	ftoa(data1,lat,4);
	ftoa(data2, lng, 4);
	sprintf(json, "{\"user\":\"HdN5SFXjEEamZksgFDpN2joyMAh66IfoBtmgRRYO\",\"lat\":\"%s\",\"lng\":\"%s\"}", lat, lng); // @suppress("Float formatting support")
	strcpy(json_test, json);
	if(simcom_at_command(json, "OK", 25000) == 1)
	{
		simcom_gprs_http_set_ssl();

		simcom_at_command("AT+HTTPACTION=1", "+HTTPACTION:", 1000);
		HAL_Delay(1000);
		free(json);
		flag = true;
	}
}
