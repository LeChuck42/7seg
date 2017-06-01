
#include <util/delay.h>
#include <stdlib.h>
#include "7seg.h"
#include <avr/eeprom.h>

#define BUFFERS	255

unsigned long stateStore[BUFFERS] EEMEM;

//static const number[10] = {};

static unsigned long calculations;
static unsigned char save;

static char primtest ( unsigned long p)
{
	unsigned long test;

	if (p < 2) return 0;
	if (p == 2) return 1;
	if (p %2 == 0) return 0;
	for (test = 3; test * test <= p; test +=2)
	{
		if (++calculations == 0x00080000UL)
		{
			save = 1;
			calculations = 0;
		}
		if (p % test == 0) return 0;
	}

	return 1;
}

int main(void) {
	static unsigned long num=1;
	unsigned long temp=1;
	static char buf[14];
	unsigned char i;
	unsigned char next=0;
	
	calculations = 0;

	Init_7seg();
	_delay_ms(100);
	while (write_str_blend("FHBOHRTLAB2012",1))
		_delay_ms(50);
	_delay_ms(3000);
	while (write_str_blend("PRIMECALCULATE",1))
		_delay_ms(50);
	_delay_ms(3000);
	

	for (i=0; i<BUFFERS; i++) {
		eeprom_read_block(&temp, &stateStore[i], sizeof(unsigned long));
		if (temp != 0xFFFFFFFF && temp > num) {
			num = temp;
			next = i+1;
		}
	}
	
	if (next == BUFFERS)
		next = 0;
	
	write_str("         ",6);
	
	while (42)
	{
		if (primtest(num))
		{
			/* display */
			ultoa(num,buf,10);
			write_str(buf,6);
			if (save) {
				eeprom_write_block(&num, &stateStore[next], sizeof(unsigned long));
				if (++next >= BUFFERS)
					next = 0;
				save = 0;
			}
			//_delay_ms(500);
		}
		num++;

	}

	for(;;) {}
}
