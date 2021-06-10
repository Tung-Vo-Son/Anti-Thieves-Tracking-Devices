/*
 * Float_String_Convert.h
 *
 *  Created on: May 24, 2021
 *      Author: Vo Son Tung HUST - Night Owl
 */

#ifndef INC_FLOAT_STRING_CONVERT_H_
#define INC_FLOAT_STRING_CONVERT_H_

#include "math.h"

void reverse(char* str, int len);
int int_to_string(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);

#endif /* INC_FLOAT_STRING_CONVERT_H_ */
