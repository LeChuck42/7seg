#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "7seg.h"

//static const number[10] = {};

static void shift_bit(unsigned char d);
static void write_digit(unsigned char seg, unsigned char data, unsigned char erase);

static void shift_bit(unsigned char d)
{
	CLR_CLK; // CLK down

	WAIT_FULL(DATA_DELAY);
	if (d)
	{
		SET_DATA;
	}
	else
	{
		CLR_DATA;
	}
	WAIT_FULL(DATA_DELAY);
	SET_CLK; // CLK up - shift bit

	WAIT_FULL(DATA_DELAY);
}

static void shift_mech(unsigned char d)
{
	CLR_CLK; // CLK down

	WAIT_FULL(MECH_DELAY);
	if (d)
	{
		SET_DATA;
	}
	else
	{
		CLR_DATA;
	}
	WAIT_FULL(MECH_DELAY);
	SET_CLK; // CLK up - shift bit

	WAIT_FULL(MECH_DELAY);
}



static void write_digit(unsigned char seg, unsigned char data, unsigned char erase)
{
	unsigned char i;

	SET_STROBE;

	WAIT_FULL(DATA_DELAY);
	// pump data

	for (i=0x80; i; i>>=1)
	{
		shift_bit(data&i);
	}

	// pump address


	for (i=0x80; i; i>>=1)
	{
		shift_bit(seg&i);
	}

	CLR_STROBE; // falling strobe, latch data

	WAIT_FULL(DATA_DELAY);


	// enable segments??
	for (i=0x80; i; i>>=1)
	{
		shift_mech(erase&i);
	}

}

static const unsigned char char_to_seg(unsigned char c)
{
	switch (c)
	{
	case '0':
		return SEG_O|SEG_OL|SEG_UL|SEG_U|SEG_UR|SEG_OR;//0xFC;
	case '1':
		return SEG_OR|SEG_UR;
	case 'Z':
	case 'z':
	case '2':
		return SEG_O|SEG_OR|SEG_M|SEG_UL|SEG_U;
	case '3':
		return SEG_O|SEG_OR|SEG_UR|SEG_U|SEG_M;
	case 'Y':
	case 'y':
	case '4':
		return SEG_OL|SEG_M|SEG_OR|SEG_UR;
	case 'S':
	case 's':
	case '5':
		return SEG_O|SEG_OL|SEG_M|SEG_UR|SEG_U;
	case '6':
		return SEG_O|SEG_OL|SEG_M|SEG_UL|SEG_U|SEG_UR;
	case '7':
		return SEG_O|SEG_OR|SEG_UR;
	case '8':
		return SEG_O|SEG_U|SEG_OL|SEG_OR|SEG_UL|SEG_UR|SEG_M;
	case '9':
		return SEG_O|SEG_OL|SEG_OR|SEG_M|SEG_UR|SEG_U;
	case 'A':
	case 'a':
		return SEG_O|SEG_OL|SEG_OR|SEG_M|SEG_UL|SEG_UR;
	case 'B':
	case 'b':
		return SEG_OL|SEG_UL|SEG_U|SEG_UR|SEG_M;
	case 'C':
	case 'c':
		return SEG_O|SEG_OL|SEG_UL|SEG_U;
	case 'D':
	case 'd':
		return SEG_OR|SEG_UR|SEG_U|SEG_UL|SEG_M;
	case 'E':
	case 'e':
		return SEG_O|SEG_OL|SEG_UL|SEG_U|SEG_M;
	case 'F':
	case 'f':
		return SEG_O|SEG_OL|SEG_M|SEG_UL;
	case 'G':
	case 'g':
		return SEG_O|SEG_OL|SEG_UL|SEG_U|SEG_UR;
	case 'H':
		return SEG_OL|SEG_OR|SEG_M|SEG_UL|SEG_UR;
	case 'h':
		return SEG_OL|SEG_UL|SEG_M|SEG_UR;
	case 'I':
	case 'i':
		return SEG_OL|SEG_UL;
	case 'J':
	case 'j':
		return SEG_OR|SEG_UR|SEG_U;
	case 'L':
	case 'l':
		return SEG_OL|SEG_UL|SEG_U;
	case 'N':
	case 'n':
		return SEG_UL|SEG_M|SEG_UR;
	case 'O':
	case 'o':
		return SEG_UR|SEG_UL|SEG_M|SEG_U;
	case 'P':
	case 'p':
		return SEG_OL|SEG_UL|SEG_O|SEG_M|SEG_OR;
	case 'R':
	case 'r':
		return SEG_UL|SEG_M;
	case 'T':
	case 't':
		return SEG_OL|SEG_UL|SEG_U|SEG_M;
	case 'U':
		return SEG_OL|SEG_OR|SEG_UL|SEG_UR|SEG_U;
	case 'u':
		return SEG_UL|SEG_UR|SEG_U;
	case '-':
		return SEG_M;
	case '°':
		return SEG_O|SEG_M|SEG_OL|SEG_OR;
	case '"':
		return SEG_OL|SEG_OR;
	case '\'':
		return SEG_OL;
	case '\\':
		return SEG_OL|SEG_M|SEG_UR;
	case '/':
		return SEG_OR|SEG_M|SEG_UL;
	case 'Ö':
	case 'ö':
		return SEG_O|SEG_M|SEG_UL|SEG_UR|SEG_U;
	case 'Ü':
	case 'ü':
		return SEG_O|SEG_UL|SEG_UR|SEG_U;
	case 'µ':
		return SEG_OL|SEG_UL|SEG_M|SEG_OR;
	case '_':
		return SEG_U;
	case ',':
		return SEG_UL;
	case '?':
		return SEG_O|SEG_OR|SEG_M|SEG_UL;
	case '=':
		return SEG_M|SEG_U;
	case '@':
		return SEG_UL|SEG_OL|SEG_O|SEG_OR|SEG_M;
	case '{':
		return SEG_UL|SEG_OL|SEG_M;
	case '}':
		return SEG_OR|SEG_UR|SEG_M;
	default:
		return 0x00;
	}
}

static void clear_seg(unsigned char seg)
{
	if (seg >= 1 && seg <= 14 )
	{
		seg <<= 4;
		write_digit(seg,0x00,0x00);
	}
}

static void set_seg(unsigned char seg, unsigned char c)
{
	if (seg >= 1 && seg <= 14 )
	{
		write_digit(seg,c,0xFF);
	}
}

static unsigned char write_str( char* str, unsigned char start)
{
	while (*str)
	{
		clear_seg(start);
		set_seg(start++,char_to_seg(*str++));
		if (start > 14)
			start = 1;
	}
	return start;
}

static unsigned char write_str_roll( char* str, unsigned char start, unsigned char end, unsigned char pos)
{
	unsigned char local_pos=pos++;
	for (;start <= end; start++, local_pos++)
	{
		if (*(str+local_pos) == 0)
		{
			local_pos=0;
		}
		clear_seg(start);
		set_seg(start,char_to_seg(*(str+local_pos)));
	}
	if (*(str+pos) == 0)
		pos = 0;
	return pos;
}
