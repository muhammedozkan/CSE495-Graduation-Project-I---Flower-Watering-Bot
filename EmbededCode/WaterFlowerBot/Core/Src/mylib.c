#include "mylib.h"

char buffer[50];
uint8_t timer_count = 0, buffer_index = 0;
volatile uint8_t automode = 0;

flowerData amount[3]={{CODE_R,1500,0},{CODE_G,3000,0},{CODE_B,4500,0}};

uint8_t string_compare(char array1[], char array2[], uint16_t length) {
	uint8_t comVAR = 0, i;
	for (i = 0; i < length; i++) {
		if (array1[i] == array2[i])
			comVAR++;
		else
			comVAR = 0;
	}
	if (comVAR == length)
		return 1;
	else
		return 0;
}

void Message_handler() {
	if (string_compare(buffer, "ON", strlen("ON"))) {
		pumpON();
		HAL_UART_Transmit(&huart2, (uint8_t*) "PUMP is ON.\n",
				strlen("PUMP is ON.\n"), 500);
	} else if (string_compare(buffer, "OFF", strlen("OFF"))) {
		pumpOFF();
		HAL_UART_Transmit(&huart2, (uint8_t*) "PUMP is OFF.\n",
				strlen("PUMP is OFF.\n"), 500);
	} else if (string_compare(buffer, "FORWARD", strlen("FORWARD"))) {
		forward();
		HAL_UART_Transmit(&huart2, (uint8_t*) "TANK is FORWARD.\n",
				strlen("TANK is FORWARD.\n"), 500);
		//HAL_Delay(1000);
		//stop();
	} else if (string_compare(buffer, "BACKWARD", strlen("BACKWARD"))) {
		backward();
		HAL_UART_Transmit(&huart2, (uint8_t*) "TANK is BACKWARD.\n",
				strlen("TANK is BACKWARD.\n"), 500);
		//HAL_Delay(1000);
		//stop();
	} else if (string_compare(buffer, "LEFT", strlen("LEFT"))) {
		turnLeft();
		HAL_UART_Transmit(&huart2, (uint8_t*) "TANK is LEFT.\n",
				strlen("TANK is LEFT.\n"), 500);
		//HAL_Delay(1000);
		//stop();
	} else if (string_compare(buffer, "RIGHT", strlen("RIGHT"))) {
		turnRight();
		HAL_UART_Transmit(&huart2, (uint8_t*) "TANK is RIGHT.\n",
				strlen("TANK is RIGHT.\n"), 500);
		//HAL_Delay(1000);
		//stop();
	} else if (string_compare(buffer, "STOP", strlen("STOP"))) {
		stop();
		HAL_UART_Transmit(&huart2, (uint8_t*) "TANK is STOP.\n",
				strlen("TANK is STOP.\n"), 500);
		//HAL_Delay(1000);
		//stop();
	} else if (string_compare(buffer, "AUTO", strlen("AUTO"))) {

		if (automode == 0) {
			automode = 1;
		} else
			{

			automode = 0;
			stop();
			}

		HAL_UART_Transmit(&huart2, (uint8_t*) "TANK is AUTOMODE ON OFF.\n",
				strlen("TANK is AUTOMODE ON OFF.\n"), 500);
		//HAL_Delay(1000);
		//stop();
	} else if (string_compare(buffer, "DATA ", strlen("DATA "))) {
		char delim[] = " ";
		char *ptr = strtok(buffer, delim);//DATA
		ptr = strtok(NULL, delim);//R
		amount[0].waterAmount=atoi(ptr);
		ptr = strtok(NULL, delim);//R
		amount[0].waterPeriod=atoi(ptr);
		ptr = strtok(NULL, delim);//G
		amount[1].waterAmount=atoi(ptr);
		ptr = strtok(NULL, delim);//G
		amount[1].waterPeriod=atoi(ptr);
		ptr = strtok(NULL, delim);//B
		amount[2].waterAmount=atoi(ptr);
		ptr = strtok(NULL, delim);//B
		amount[2].waterPeriod=atoi(ptr);


		HAL_UART_Transmit(&huart2, (uint8_t*) "DATA ACCEPTED.\n",
				strlen("DATA ACCEPTED.\n"), 500);
		//HAL_Delay(1000);
		//stop();
	}

	else {
		strcat(buffer, "\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), 500);
	}

	memset(buffer, 0, sizeof(buffer));
	buffer_index = 0;
	timer_count = 0;
}
