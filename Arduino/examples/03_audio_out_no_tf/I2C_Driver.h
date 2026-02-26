#pragma once
#include "Wire.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_MASTER_NUM    I2C_NUM_0 
#define I2C_SCL_PIN       10
#define I2C_SDA_PIN       11

void I2C_Init(void);

bool I2C_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length);
bool I2C_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length);
bool I2C_Write_Not_REG(uint8_t Driver_addr, const uint8_t *Reg_data, uint32_t Length);

#ifdef __cplusplus
}
#endif
