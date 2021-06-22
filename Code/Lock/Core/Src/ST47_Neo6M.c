/*
 * ST47_Neo6M.c
 *
 *  Created on: Apr 24, 2021
 *      Author: Vo Son Tung HUST
 */


#include "ST47_Neo6M.h"
GPS_Struct gps;

float convert(float location)
{
	 float degrees = floor(location / 100);
	 double minutes = location - (100 * degrees);
	 minutes /= 60;
	 degrees += minutes;
	 return degrees;
}

void neo_init()
{
	gps.index = 0;
	neo_on();
	HAL_UART_Receive_IT(&huart1, &gps.rx, 1);
}

void neo_on()
{
	HAL_GPIO_WritePin(GPS_VOLTAGE_GPIO_Port, GPS_VOLTAGE_Pin, GPIO_PIN_SET);
}

void neo_off()
{
	HAL_GPIO_WritePin(GPS_VOLTAGE_GPIO_Port, GPS_VOLTAGE_Pin, GPIO_PIN_RESET);
}

void neo_callback()
{
	if(gps.rx == '\n')
	{
		gps.flag = true;
		gps.index = 0;
	}
	else gps.buffer[gps.index ++] = gps.rx;
	HAL_UART_Receive_IT(&huart1, &gps.rx, 1);
}

bool neo_gps_message_comleted()
{
	return gps.flag;
}

void neo_gps_process_data(char* buffer)
{
	if(neo_gps_message_comleted() == true)
	{
//		HAL_UART_Transmit(&debug, gps.buffer, sizeof(gps.buffer), 2000);
		char* response = malloc(strlen(gps.buffer) + 1);
		strcpy(response, gps.buffer);
		if(response != NULL)
		{
			char* token = strtok(response, ",");
			strcpy(gps.gpgga.time, strtok(NULL, ","));
			gps.gpgga.latitude = atof(strtok(NULL, ","));
			gps.gpgga.ns_indicator = strtok(NULL, ",");
			gps.gpgga.longtitude = atof(strtok(NULL, ","));
			gps.gpgga.ew_indicator = strtok(NULL, ",");
		}
		free(response);
		gps.flag = false;

	}
}

float neo_get_gps_latitude()
{
	return convert(gps.gpgga.latitude);
}

float neo_get_gps_longitude()
{
	return convert(gps.gpgga.longtitude);
}
