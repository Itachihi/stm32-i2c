#ifndef __I2C1_H__
#define __I2C1_H__

#include <stdint.h>

void I2C1_Init(void);
int I2C1_Write(int8_t addr, const uint8_t *dat, int len);
int I2C1_Read(uint8_t addr, uint8_t *dat, int len);

#endif
