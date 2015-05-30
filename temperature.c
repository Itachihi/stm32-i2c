#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include "temperature.h"
#include "protocol.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "gsm.h"
#include "stm32f10x_usart.h"
#include <string.h>
#include "i2c1.h"
#include <stdlib.h>

#define UART_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE + 256 )
#define TERM_UPLOAD_DATA_TIME   (configTICK_RATE_HZ * 60 * 5)
#define __TIMES__    10

static xQueueHandle __queue;

float readTemperature(void) {
	float temp;
	uint8_t buf[2];
	buf[0] = LM75_TEMP_REG;
	I2C1_Write(0x48, buf, 1);
	I2C1_Read(0x48, buf, 2);
	temp = ((buf[0] << 8) + buf[1]) >> 5;
	if (temp > 0x3ff) {
		temp = 0.125 * (temp - 2048);
	} else {
		temp *= 0.125;
	}

	printf("temp:%.3f\r\n", temp);
	return temp;
}

static void __task(void *unused) {
	uint8_t index = 0;
	float temperature = 0;
	uint32_t vb = 0;
	int size;

	__queue = xQueueCreate(20, sizeof(char));

	I2C1_init();

	while (1) {
		float thisTemp = 0;

		vTaskDelay(TERM_UPLOAD_DATA_TIME / __TIMES__);

		thisTemp = readTemperature();

		temperature += thisTemp;
		++index;
		if (index < __TIMES__) {
			continue;
		}

	  	temperature  /=  index ;

		printf("temperature:%.1f\r\n", temperature);
		index = 0;
		temperature = 0;
	}
}

void TemperatureInit() {
	xTaskCreate(__task, "TEMPERATRUE", UART_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 5, NULL);
}
