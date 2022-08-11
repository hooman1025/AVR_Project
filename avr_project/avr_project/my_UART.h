#ifndef __MY_UART_H_
#define __MY_UART_H_

#define F_CPU 14745600L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void UART_INIT(void);
void UART0_transmit(char data_com);
void UART1_transmit(char data_ardu);
uint8_t UART0_receive(void);
char UART1_receive(void);
void UART0_printString(char str[]);
void UART1_printString(char str[]);





#endif /* MY_UART_H_ */