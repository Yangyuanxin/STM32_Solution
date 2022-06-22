#ifndef	_AT24C02_I2C_DRV_H_
#define	_AT24C02_I2C_DRV_H_

#include "stm32l4xx_hal.h"

#define	AT24C02_ADDR_WRITE	0xA0
#define	AT24C02_ADDR_READ	0xA1


int8_t At24c02_Write_Byte(uint16_t addr, uint8_t* dat,uint16_t size);
uint8_t At24c02_Read_Byte(uint16_t addr, uint8_t* read_buf,uint16_t size);


#endif	/* _AT24C02_I2C_DRV_H_ */
