#include "softi2c.h"
#include "stm32f10x_gpio.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "i2c1.h"

void __init() {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   	//50M
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;       //¿ªÂ©Êä³ö
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

bool __sck(bool b) {
	if (b) {
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
	} else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	}

	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
}


bool __sda(bool b) {
	if (b) {
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
	} else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	}

	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
}


static const SoftI2C __i2c1 = {
	__init, __sck, __sda,
};

static xSemaphoreHandle __semaphore = NULL;

int I2C1_Init() {
	if (__semaphore == NULL) {
		vSemaphoreCreateBinary(__semaphore);
	}
	SoftI2C_Init(&__i2c1);
}

int I2C1_Write(int8_t addr, const uint8_t *dat, int len) {
	int ret;
	xSemaphoreTake(__semaphore, portMAX_DELAY);
	ret = SoftI2C_Write(&__i2c1, addr, dat, len);
	xSemaphoreGive(__semaphore);
	return ret;
}

int I2C1_Read(uint8_t addr, uint8_t *dat, int len) {
	int ret;
	xSemaphoreTake(__semaphore, portMAX_DELAY);
	ret = SoftI2C_Read(&__i2c1, addr, dat, len);
	xSemaphoreGive(__semaphore);
	return ret;
}


