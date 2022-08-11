/*
 * avr_project.c
 *
 * Created: 2022-07-28 오전 9:16:03
 * Author : KCCI07
 */ 
#define F_CPU	14745600L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "my_UART.h"
#include "i2c.h"
#include "sht20.h"
#include "lcd.h"

char buff[20] = { 0 };
int index = 0;
int flag = 0;

int parsing(void);

void adcinit(void)
{
	DDRF = 0x01;
	ADCSRA |= (1<< ADPS2)|(1<< ADPS1)|(1<< ADPS0); // prescale 128
	ADMUX |= 0X00; // AREF 5V
	ADCSRA |= (1<<ADEN);
}

uint16_t readAdc(uint8_t channel)
{
	ADMUX |= channel;
	ADCSRA |= (1<<ADSC); // adc start
	while(ADCSRA&(1<<ADSC)); //wait adc
	
	return ADCW; // ADCH, ADCL
}

void Timer1Init()
{
	DDRB|=0x20;
	PORTB|=0x20;
	TCCR1A=0x82;
	TCCR1B=0x1b;
	ICR1=4999;     //TOP
	OCR1A=375;     //0도	
}

void LEDInit()
{
	DDRB |= 0x0f; // 4~7
	DDRG |= 0x0f; // 0~3
}
void LED_Toggle(void)
{
	PORTB |= 0x0f;
	PORTG |= 0x0f;
	_delay_ms(100);
	PORTB &= 0x00;
	PORTG &= 0x00;
	_delay_ms(100);
}

void Servo_On(void)
{
	OCR1A=250;     //-90도
	_delay_ms(1000);
	//OCR1A=500;     //90도
	//_delay_ms(1000);
}

void Servo_Off(void)
{
	//OCR1A=250;     //-90도
	//_delay_ms(1000);
	OCR1A=500;     //90도
	_delay_ms(1000);
}
ISR(USART1_RX_vect)
{
	char msg = UDR1;
	if(msg == 'E')
	{
	
	}
	else if(msg != 'L')
	{
		//UART0_transmit((char)index);
		buff[index] = msg;
		//UART0_transmit(buff[index]);
		index++;
		//UART0_transmit('\n');
	}
	else 
	{
		buff[index] = 'L';
		index = 0;
		flag = 1; 
	}
	//_delay_ms(1);
}


int main(void)
{
	UART_INIT();
	I2C_Init();
	adcinit();
	LCDInit(0x02);
	Timer1Init();
	LEDInit();
	sei();
	
	int ppm;
	float humi, temp;
	char ValueString[100];
	int ret;
	
	int Emer_value = 0;
	
	while (1)
	{
		humi = readHumidity();
		temp = readTemperature();
		ppm = readAdc(0);
		
		
		if(flag == 1)
		{
			ret = parsing();
			if(ret == 1)
			UART0_transmit('a');
			else if(ret == 2)
			UART0_transmit('b');
			else if(ret == 3)
			UART0_transmit('c');
			
			//if(ppm > 10)
			//{
			//	UART1_transmit('E');
			//}
		}
		UART0_transmit('\n');
		if(ret==1)
			Servo_Off();
		if(ret == 2)
			Servo_On();
		if(ret ==3)
		{
			Servo_On();
			LED_Toggle();
		}
		
		//sprintf(ValueString, "humi : %d , temp : %d , PPM : %d \n", (int)humi, (int)temp, ppm);
		//UART0_printString(ValueString);
		
		LCD_Clear();
		LCDWriteStringXY(0,0,"humi:");
		LCDWriteIntXY(5,0,(int)humi,2);
		LCDWriteStringXY(9,0,"temp:");
		LCDWriteIntXY(14,0,(int)temp,2);
		LCDWriteStringXY(0,1,"ppm:");
		LCDWriteIntXY(4,1,ppm,2);
		
		//LED_Toggle();
		

	}
}

int parsing(void)
{
	int str_len, door_len, state_len;
	char *pArray[4] = {0};
	char *pToken = NULL;
	int i;
	char *ptr_door = NULL, *ptr_state = NULL;
	static char door[10] = { 0 }, state_char[10] = { 0 };
	int state = 0;
	char ssparsing[30] = { 0 };
	
	
	
	str_len = strlen(buff);
	pToken = strtok(buff, "#");
		
	i = 0;
	while(pToken != NULL) {
		pArray[i] = pToken;
		pToken = strtok(NULL, "L");
		i++;
	}
		
	ptr_door = pArray[0];
	ptr_state = pArray[1];
		
	door_len = strlen(ptr_door);
	state_len = strlen(ptr_state);

	if(door_len > 0) {
		for (int i = 0; i <= door_len; i++)
		door[i] = *(ptr_door + i);
	}
		
	if(state_len > 0) {
		for (int i = 0; i <= state_len; i++)
		state_char[i] = *(ptr_state + i);
	}
	state = atoi(state_char);
		
		
	sprintf(ssparsing, "%s,%d\n", door, state);
		
	i = 0;
		
	while(ssparsing[i] != '\0')
	{
		UART0_transmit(ssparsing[i]);
		i++;
	}
		
	for(i = 0; i <= str_len; i++)
	buff[i] = 0;
	flag = 0;
	if(state == 0)
	{
		if(!strcmp(door, "close")) // close, 0
			return 1;
		else if(!strcmp(door, "open")) //open, 0
			return 2; 
	}
	else if(state == 1)
		return 3;
	else
		return 0;
		/*
	if(!(strcmp(door, "close")) && state == 0)
	return 1;
	else if(!(strcmp(door, "open")) && state == 0)
	return 2;
	else if(!(strcmp(door, "open")) && state == 1)
	return 3;
	*/
	
}