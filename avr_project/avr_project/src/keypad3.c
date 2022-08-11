char Keypad_input(void);

char Keypad_input(void)
{
	char input=' ';
	
	DDRF=0x0f;
	PORTF = 0xff;

	PORTF = 0b11111110;
	_delay_us(1);
	if ((PINF & 0x10) == 0) input = '1';
	else if ((PINF & 0x20) == 0) input = '2';
	else if ((PINF & 0x40) == 0) input = '3';

	PORTF = 0b11111101;
	_delay_us(1);
	if ((PINF & 0x10) == 0) input = '4';
	else if ((PINF & 0x20) == 0) input = '5';
	else if ((PINF & 0x40) == 0) input = '6';
	
	PORTF = 0b11111011;
	_delay_us(1);
	if ((PINF & 0x10) == 0) input = '7';
	else if ((PINF & 0x20) == 0) input = '8';
	else if ((PINF & 0x40) == 0) input = '9';

	PORTF = 0b11110111;
	_delay_us(1);
	if ((PINF & 0x10) == 0) input = '*';
	else if ((PINF & 0x20) == 0) input = '0';
	else if ((PINF & 0x40) == 0) input = '#';

	return input;                               // 입력이 없으면 공백문자 리턴

}
*/