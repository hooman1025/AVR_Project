/*
 * keypad.c
 *
 * Created: 2022-07-29 오전 9:56:10
 *  Author: KCCI07
 */ 
#include "keypad.h"

char KeySet(void)
{
	char KeyBuff = 'n';  // 함수 내에서 사용될 버퍼 선언, 초기화 ( else KeyBuff = 'n' )
	
	DDRF = 0x00; PORTF=0xff;  // PC 초기화
	
	DDRF = 0x01; PORTF &= ~0x01;
	_delay_us(5);
	if(!(PINF&0x10)) KeyBuff = '1';
	if(!(PINF&0x20)) KeyBuff = '2';
	if(!(PINF&0x40)) KeyBuff = '3';
	//if(!(PINF&0x80)) KeyBuff = 'A';
	DDRF=0x00; PORTF = 0xff;                   // 1 2 3 A 행 검사 끝
	
	DDRF = 0x02; PORTF &= ~0x02;
	_delay_us(5);
	if(!(PINF&0x10)) KeyBuff = '4';
	if(!(PINF&0x20)) KeyBuff = '5';
	if(!(PINF&0x40)) KeyBuff = '6';
	//if(!(PINF&0x80)) KeyBuff = 'B';
	DDRF = 0x00; PORTF = 0xff;                 // 4 5 6 B 행 검사 끝
	
	DDRF = 0x04; PORTF &= ~0x04;
	_delay_us(5);
	if(!(PINF&0x10)) KeyBuff = '7';
	if(!(PINF&0x20)) KeyBuff = '8';
	if(!(PINF&0x40)) KeyBuff = '9';
	//if(!(PINF&0x80)) KeyBuff = 'C';
	DDRF = 0x00; PORTF = 0xff;                 // 7 8 9 C 행 검사 끝
	
	DDRF = 0x08; PORTF &= ~0x08;
	_delay_us(5);
	if(!(PINF&0x10)) KeyBuff = '*';
	if(!(PINF&0x20)) KeyBuff = '0';
	if(!(PINF&0x40)) KeyBuff = '#';
	//if(!(PINF&0x80)) KeyBuff = 'D';
	DDRF = 0x00; PORTF = 0xff;                 // * 0 # D 행 검사 끝
	
	return KeyBuff;
}