#include "sht20.h"

uint8_t checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor)
{
	uint32_t remainder = (uint32_t)message_from_sensor << 8;
	remainder |= check_value_from_sensor;
	uint32_t divsor = (uint32_t)SHIFTED_DIVISOR;
	for(int i = 0 ; i < 16 ; i++){
		if(remainder & (uint32_t)1 << (23 - i)){
			remainder ^= divsor;
		}
		divsor >>= 1;
	}
	return (uint8_t)remainder;
}

uint16_t readValue(unsigned char cmd)
{
	unsigned char toRead;
	unsigned char counter;
	unsigned char msb, lsb, checksum;
	uint16_t rawValue;
	
	I2C_Start();
	I2C_Write_SLAW(SLAVE_ADDRESS);
	I2C_Write(cmd);
	_delay_us(20);
	I2C_Stop();
	
	for(counter =0; counter < MAX_COUNTER ; counter++)
	{
		_delay_ms(DELAY_INTERVAL);
		I2C_Start();
		toRead = I2C_Write_SLAR(SLAVE_ADDRESS);
		if(toRead == 0)
		break;
		else if(toRead ==1)
		I2C_Stop();
	}
	if(counter == MAX_COUNTER)
	{
		return ERROR_I2C_TIMEOUT;
	}
	
	msb = I2C_Read();
	lsb = I2C_Read();
	checksum = I2C_Read();
	I2C_Stop();
	rawValue = ((uint16_t) msb << 8) | (uint16_t) lsb;
	
	if(checkCRC(rawValue, checksum) != 0){
		return ERROR_BAD_CRC;
	}
	
	return (rawValue& 0xFFFC);
}

float readHumidity(void)
{
	float temp;
	float rh;

	uint16_t rawHumidity = readValue(TRIGGER_HUMD_MEASURE_NOHOLD);
	if(rawHumidity == ERROR_I2C_TIMEOUT || rawHumidity == ERROR_BAD_CRC){
		return(rawHumidity);
	}
	temp = rawHumidity / 65536.0;
	temp = temp * 125.0;
	rh = temp - 6.0;

	return rh;
}

float readTemperature(void)
{
	float temp;
	float realTemperature;
	
	uint16_t rawTemperature = readValue(TRIGGER_TEMP_MEASURE_NOHOLD);
	if(rawTemperature == ERROR_I2C_TIMEOUT || rawTemperature == ERROR_BAD_CRC){
		return(rawTemperature);
	}
	temp = rawTemperature / 65536.0;
	temp = temp * 175.72;
	realTemperature = temp - 46.85;

	return realTemperature;
}
