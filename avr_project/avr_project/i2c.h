#ifndef _I2C_H
#define _I2C_H

#include <avr/io.h>
#include <util/delay.h>

#define ExtDev_ERR_MAX_CNT 2000

#define I2C_START   0x08
#define I2C_RESTART 0x10
#define MT_SLA_ACK 0x18
#define MT_DATA_ACK 0x28
#define MR_SLA_ACK 0x40
#define MR_DATA_ACK 0x58

unsigned char I2C_TransCheck_ACK(unsigned char Stat);
void I2C_Init(void);
unsigned char I2C_Start(void);
void I2C_Stop(void);
unsigned char TWI_Write_SLAW(unsigned char Addr);
unsigned char TWI_Write_SLAR(unsigned char Addr);
unsigned char I2C_Write(uint8_t var_i2cData_u8);
uint8_t I2C_Read(void);


#endif

