#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "7-seg.h"



int main(void) {
	unsigned char i, pos=0;
	unsigned int cnt=0;
	char buf[15];
	DDRD |= 0x07;
	PORTA = 0;

	for (i=1; i<=14; i++)
		clear_seg(i);

	_delay_ms(1000);
	write_str("RULEZ", 6);

	while (42)
	{
		itoa(cnt,buf,10);
		cnt++;
		if (cnt > 9999)
			cnt = 0;
		write_str(buf,11);
		for (i=0;i<4;i++)
		{
			pos = write_str_roll("FH-BOCHOLT RT-LABOR 25-10-2012    BLBLB    och nico    ", 1, 5, pos);
			_delay_ms(250);
		}
	}

	//_delay_ms(100);
	//write_digit(0x02,0x01);

	for(;;) {}
}
