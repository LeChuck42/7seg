
#include <util/delay.h>
#include "7seg.h"

//static const number[10] = {};



int main(void) {
	//unsigned char i, pos=0, pos2=0;
	//unsigned int cnt=0;
	//volatile unsigned int delay;
	//char buf[15];
	//char muster[] = {SEG_U, SEG_M, SEG_O, SEG_O, SEG_M, SEG_U,0};

	Init_7seg();
	_delay_ms(100);
	while (42)
	{
		/*
		itoa(cnt,buf,10);
		cnt++;
		if (cnt > 9999)
			cnt = 0;
		write_str(buf,11);
		for (i=0;i<4;i++)
		{
		   pos = write_str_roll("FH-BOCHOLT RT-LABOR 25-10-2012    BLBLB    och nico    ", 1, 5, pos);
		   pos2 = write_seg_roll(muster, 6, 10, pos2);
			_delay_ms(250);
		}
		*/
		/*
		for (delay=rand(); delay; delay--);
		cnt = rand();
		write_seg((((unsigned char)cnt) % 14) + 1, (cnt >> 7)&SEG_ALL);
		*/
		while (write_str_blend("FHBOHRTLAB2012",1))
			_delay_ms(50);
		_delay_ms(3000);
		while (write_str_blend("0123456789abcd",1))
			_delay_ms(50);
		_delay_ms(3000);
		while (write_str_blend("blblb31337beef",1))
			_delay_ms(50);
		_delay_ms(3000);
		while (write_str_blend("pcb  built1988",1))
			_delay_ms(50);
		_delay_ms(3000);



	}

	//_delay_ms(100);
	//write_digit(0x02,0x01);

	for(;;) {}
}
