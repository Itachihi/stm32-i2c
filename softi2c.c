#include "softi2c.h"


void  inline __start(const SoftI2C *i2c) {
	i2c->sck(1);
	i2c->sda(1);
	i2c->sda(0);
	i2c->sck(0);
}

void  inline __stop(const SoftI2C *i2c) {
	i2c->sda(0);
	i2c->sck(1);
	i2c->sda(1);
}

bool inline __onebit(const SoftI2C *i2c, bool bit) {
	uint32_t timeout = 0;
	i2c->sda(bit);
	while (i2c->sck(1) == 0) {
		if (timeout++ > 1000) {
			break;
		}
	}
	bit = i2c->sda(bit);
	i2c->sck(0);
	return bit;
}

uint8_t inline __onebyte(const SoftI2C *i2c, uint8_t dat) {
	uint8_t ret;
	uint8_t bit;
	for (ret = 0, bit = 0x80; bit != 0x00; bit = bit >> 1) {
		if (__onebit(i2c, dat & bit)) {
			ret |= bit;
		}
	}
	return ret;
}


void SoftI2C_Init(const SoftI2C *i2c) {
	i2c->init();
	i2c->sda(1);
	i2c->sck(1);
}



int SoftI2C_Write(const SoftI2C *i2c, uint8_t addr, const uint8_t *dat, int len) {
	int i;

	__start(i2c); // start
	__onebyte(i2c, addr << 1); // addr + Write
	if (0 != __onebit(i2c, 1)) { // check ACK
		return 0;
	}

	for (i = 0; i < len; ++i) {
		__onebyte(i2c, *dat++);
		if (0 != __onebit(i2c, 1)) { // check ACK
			break;
		}
	}
	__stop(i2c);
	return i;
}

int SoftI2C_Read(const SoftI2C *i2c, uint8_t addr, uint8_t *dat, int len) {
	int i;

	__start(i2c); // start
	__onebyte(i2c, (addr << 1)  + 1); // addr + Write
	if (0 != __onebit(i2c, 1)) { // check ACK
		return 0;
	}

	for (i = 0; i < len; ++i) {
		*dat++ = __onebyte(i2c, 0xFF);
		__onebit(i2c, 0); // ACK
	}
	__stop(i2c);
	return i;
}
