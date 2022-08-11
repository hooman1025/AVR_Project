/*
 * keypad.h
 *
 * Created: 2022-07-29 오전 10:21:46
 *  Author: KCCI07
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define F_CPU 14745600L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define pKEYPAD_OUT_A_HIGH        PORTF |= 0x01
#define pKEYPAD_OUT_A_LOW        PORTF &= 0xFE
#define pKEYPAD_OUT_B_HIGH        PORTF |= 0x02
#define pKEYPAD_OUT_B_LOW        PORTF &= 0xFD
#define pKEYPAD_OUT_C_HIGH        PORTF |= 0x04
#define pKEYPAD_OUT_C_LOW        PORTF &= 0xFB
#define pKEYPAD_IN            (PINF>>4)

char KeypadIn(void);

#endif /* KEYPAD_H_ */