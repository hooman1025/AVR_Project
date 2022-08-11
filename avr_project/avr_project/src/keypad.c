/*
 * keypad.c
 *
 * Created: 2022-07-29 오전 10:21:55
 *  Author: KCCI07
 */ 

#include "keypad.h"

char KeypadIn(void)
{
	char d;
	
	DDRF=0x00;
	PORTF=0xff;
	
	// 1열 읽기Q
	pKEYPAD_OUT_A_LOW;
	pKEYPAD_OUT_B_HIGH;
	pKEYPAD_OUT_C_HIGH;
	
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	
	if(d==0x0E){
		return '1';
	}
	if(d==0x0D){
		return '4';
	}
	if(d==0x0B){
		return '7';
	}
	if(d==0x07){
		return '*';
	}
	
	// 2열 읽기
	pKEYPAD_OUT_A_HIGH;
	pKEYPAD_OUT_B_LOW;
	pKEYPAD_OUT_C_HIGH;
	
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	
	if(d==0x0E){
		return '2';
	}
	if(d==0x0D){
		return '5';
	}
	if(d==0x0B){
		return '8';
	}
	if(d==0x07){
		return '0';
	}
	
	// 3열 읽기
	pKEYPAD_OUT_A_HIGH;
	pKEYPAD_OUT_B_HIGH;
	pKEYPAD_OUT_C_LOW;
	
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	d = pKEYPAD_IN;
	
	if(d==0x0E){
		return '3';
	}
	if(d==0x0D){
		return '6';
	}
	if(d==0x0B){
		return '9';
	}
	if(d==0x07){
		return '#';
	}
	return 0;
}
