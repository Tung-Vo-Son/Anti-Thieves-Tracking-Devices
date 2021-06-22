/*
 * ST47_Neo6M.h
 *
 *  Created on: Apr 24, 2021
 *      Author: Vo Son Tung HUST
 */

#ifndef INC_ST47_NEO6M_H_
#define INC_ST47_NEO6M_H_

#include "main.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern UART_HandleTypeDef huart1;


typedef struct
{
	/* Header */

	/* UTC Time */
	char time[10];

	/* Location */
	float latitude;
	char*  ns_indicator;
	float longtitude;
	char*  ew_indicator;

}GPGGA_Struct;

typedef struct
{
	uint8_t rx;
	uint8_t index;
	uint8_t buffer[100];
	bool flag;

	GPGGA_Struct gpgga;

}GPS_Struct;

void neo_init();
void neo_on();
void neo_off();
void neo_callback();
bool neo_gps_message_comleted();
void neo_gps_process_data();
float convert(float degMin);
float neo_get_gps_latitude();
float neo_get_gps_longitude();

#endif /* INC_ST47_NEO6M_H_ */
