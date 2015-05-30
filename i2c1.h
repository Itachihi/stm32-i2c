#ifndef __I2C1_H__
#define __I2C1_H__

#include <stdint.h>

#define  GPIOn    GPIOB
#define  SDA_PIN  GPIO_Pin_7
#define  SCL_PIN  GPIO_Pin_6

int I2C1_Write(int8_t addr, const uint8_t *dat, int len);
int I2C1_Read(uint8_t addr, uint8_t *dat, int len);

#endif
