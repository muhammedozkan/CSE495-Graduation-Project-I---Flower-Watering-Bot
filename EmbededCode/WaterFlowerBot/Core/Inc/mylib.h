/*
 * mylib.h
 *
 *  Created on: Nov 30, 2021
 *      Author: muham
 */

#ifndef INC_MYLIB_H_
#define INC_MYLIB_H_

#include "main.h"
#include <string.h>

#define CODE_R 82 //R
#define CODE_G 74 //G
#define CODE_B 66 //B

typedef struct flowerData{
	uint8_t flowerCode;
	uint32_t waterAmount;
	uint32_t waterPeriod;
}flowerData;

extern char buffer[50];
extern uint8_t timer_count, buffer_index;
volatile extern uint8_t automode;
extern UART_HandleTypeDef huart2;

extern flowerData amount[3];

uint8_t string_compare(char array1[], char array2[], uint16_t length);
void Message_handler();

#endif /* INC_MYLIB_H_ */
