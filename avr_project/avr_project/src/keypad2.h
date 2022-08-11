/*
 * keypad.h
 *
 * Created: 2022-07-29 오전 9:56:19
 *  Author: KCCI07
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define F_CPU 14745600L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

char KeySet(void);

#endif /* KEYPAD_H_ */