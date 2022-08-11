#include "i2c.h"
#define I2C_SCL	PD0
#define I2C_SDA	PD1

unsigned char I2C_TransCheck_ACK(unsigned char Stat)
{
	unsigned int ExtDev_ErrCnt = 0;
	while (!(TWCR & (1<<TWINT)));         // 패킷 전송 완료될 때 까지 wait
	{
		if(ExtDev_ErrCnt++>ExtDev_ERR_MAX_CNT){ return 1; }
	}
	if ((TWSR & 0xf8) != Stat) return 1;  // 전송 검사(ACK) : error시 1 반환
	else return 0;
}

void I2C_Init(void)
{
	DDRD |= (1<< I2C_SCL)|(1<< I2C_SDA);
	TWBR=32; //SCL frequency is 200kHz
	TWSR=0x00; //set presca1er bits to zero
}

unsigned char I2C_Start(void)
{
	TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	 return I2C_TransCheck_ACK(I2C_START);
}

void I2C_Stop(void)
{
	TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	//_delay_us(100) ; //wait for a short time
}
/***************************************/

unsigned char I2C_Write_SLAW(unsigned char Addr)
{
	TWDR = Addr;
	TWCR = (1<<TWINT) | (1<<TWEN);      // SLA + W 패킷 보내기
	return I2C_TransCheck_ACK(MT_SLA_ACK);
}

unsigned char I2C_Write_SLAR(unsigned char Addr)
{
	TWDR = Addr|0x01;
	TWCR = (1<<TWINT) | (1<<TWEN);  // SLA + R 패킷 보내기
	return I2C_TransCheck_ACK(MR_SLA_ACK);
}

unsigned char I2C_Write(uint8_t var_i2cData_u8)
{
	TWDR = var_i2cData_u8 ;
	TWCR = ((1<< TWINT) | (1<<TWEN));
	return I2C_TransCheck_ACK(MT_DATA_ACK); 
}

uint8_t I2C_Read(void)
{
	TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWEA));
	while ( !(TWCR & (1 <<TWINT))); // wait recieve complete
	I2C_TransCheck_ACK(MR_DATA_ACK);
	return TWDR;
}