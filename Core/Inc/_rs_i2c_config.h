#ifndef I2C_CONF
#define I2C_CONF

#include "stm32f4xx.h"

void I2C1_Init(void);                  													// I2C1 modülünü başlatan fonksiyon prototipi
void I2C1_Write(uint8_t slaveAddr, uint8_t *pData, uint32_t size); 						// I2C ile birden fazla byte veri yazan fonksiyon prototipi
void I2C1_WriteByte(uint8_t slaveAddr, uint8_t reg, uint8_t data); 						// I2C ile tek byte veri (belirli bir register'a) yazan fonksiyon prototipi
void I2C1_ReadByte(uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData);					// I2C ile tek byte veri okuyan fonksiyon prototipi
void I2C1_ReadBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint32_t size);	// I2C ile birden fazla byte veri okuyan fonksiyon prototipi

#endif
