#include "at24c02_i2c_drv.h"
#include "i2c.h"

/**
 * @brief		AT24C02任意地址写一个字节数据
 * @param		addr —— 写数据的地址（0-255）
 * @param		dat  —— 存放写入数据的地址
 * @param		size  —— 要写入数据的个数
 * @retval		成功 —— HAL_OK
*/
int8_t At24c02_Write_Byte(uint16_t addr, uint8_t* dat,uint16_t size)
{
		uint16_t i = 0, temp = 0;
    HAL_StatusTypeDef ret = HAL_ERROR ;
    temp = size / (uint16_t)0x08;
    for(i = 0; i < temp; i++)
    {
        ret = HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr + (i * 8), I2C_MEMADD_SIZE_8BIT, dat + (i * 8), 8, 0xFFFF);
        HAL_Delay(10);
    }
    temp = size % (uint16_t)0x08;
    if(temp)
    {
        HAL_Delay(10);
        ret = HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr + (i * 8), I2C_MEMADD_SIZE_8BIT, dat + (i * 8), temp, 0xFFFF);
    }

    if(HAL_ERROR == ret)
        return -1 ;
    return HAL_OK ;
}
/**
 * @brief		AT24C02指定地址开始读出指定个数的数据
 * @param		addr —— 读数据的地址（0-255）
 * @param		read_buf —— 存放读取数据的地址
 * @param   size —— 要读出数据的个数
 * @retval		成功 —— HAL_OK
*/
uint8_t At24c02_Read_Byte(uint16_t addr, uint8_t* read_buf, uint16_t size)
{
    return HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR_READ, addr, I2C_MEMADD_SIZE_8BIT, read_buf, size, 0xFFFF);
}

