#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	void (* init)(void);
	bool (* sck)(bool b);
	bool (* sda)(bool b);
} SoftI2C;

void SoftI2C_Init(const SoftI2C *i2c);
int SoftI2C_Write(const SoftI2C *i2c, uint8_t addr, const uint8_t *dat, int len);
int SoftI2C_Read(const SoftI2C *i2c, uint8_t addr, uint8_t *dat, int len);

#endif