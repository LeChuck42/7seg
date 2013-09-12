#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/delay.h>


#define MECH_DELAY	20
#define DATA_DELAY	1

#define WAIT_QUAD(d) _delay_us(d)
#define WAIT_HALF(d) _delay_us(2*d)
#define WAIT_FULL(d) _delay_us(4*d)


#define SET_CLK		PORTD |= 0x01
#define CLR_CLK		PORTD &= ~0x01

#define SET_DATA	PORTD |= 0x02
#define CLR_DATA	PORTD &= ~0x02

#define SET_STROBE	PORTD |= 0x04
#define CLR_STROBE	PORTD &= ~0x04


#define SEG_O	0x80
#define SEG_OR	0x40
#define SEG_UR	0x20
#define SEG_U	0x10
#define SEG_UL	0x08
#define SEG_OL	0x04
#define SEG_M	0x02

static void shift_bit(unsigned char d);
static void write_digit(unsigned char seg, unsigned char data, unsigned char erase, unsigned char skip);
static void shift_mech();
static void skip_bit();
static void clear_digit(unsigned char digit, unsigned char skip);
static void set_digit(unsigned char digit, unsigned char c, unsigned char skip);


static unsigned char content[14];

static void shift_bit(unsigned char d)
{
	CLR_CLK; // CLK down

	if (d)
	{
		SET_DATA;
	}
	else
	{
		CLR_DATA;
	}

	_delay_us(DATA_DELAY);
	SET_CLK; // CLK up - shift bit
	_delay_us(DATA_DELAY);
}

static void skip_bit()
{
	SET_CLK; // CLK down
	_delay_us(1);
	CLR_CLK; // CLK up - shift bit
	_delay_us(1);
}

static void shift_mech()
{
	SET_CLK; // CLK down
	WAIT_HALF(MECH_DELAY);
	CLR_CLK; // CLK up - shift bit
	WAIT_HALF(MECH_DELAY);
}



static void write_digit(unsigned char seg, unsigned char data, unsigned char erase, unsigned char skip)
{
	static const unsigned char delete_order[7]={SEG_U,SEG_UR,SEG_UL,SEG_M,SEG_OR,SEG_OL,SEG_O};
	static const unsigned char set_order[7]={SEG_O,SEG_OR,SEG_UR,SEG_U,SEG_UL,SEG_OL,SEG_M};

	unsigned char i;

	if ((skip & SEG_ALL) != SEG_ALL)
	{
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

		WAIT_HALF(DATA_DELAY);

		CLR_CLK;

		WAIT_HALF(DATA_DELAY);

		// enable segments??
		if (erase)
		{
			CLR_DATA;
			for (i=0; i<7; i++)
			{
				if (skip&delete_order[i])
				{
					skip_bit();
				}
				else
				{
					shift_mech();
				}
			}
		}
		else
		{
			SET_DATA;
			for (i=0; i<7; i++)
			{
				if (skip&set_order[i])
				{
					skip_bit();
				}
				else
				{
					shift_mech();
				}
			}
		}
		SET_CLK;
	}
}

void Init_7seg()
{
	unsigned char i;
	DDRD |= 0x07;
	PORTA = 0;
	for (i=0; i<14; i++)
		content[i] = SEG_ALL;
	for (i=1; i<=14; i++)
		clear_digit(i, 0);

}

const unsigned char char_to_seg(unsigned char c)
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

static void clear_digit(unsigned char digit, unsigned char skip)
{
	if (digit >= 1 && digit <= 14 )
	{
		skip |= ~content[digit-1];
		write_digit(digit<<4, 0x00, 1, skip);
		content[digit-1] &= skip;
	}
}

static void set_digit(unsigned char digit, unsigned char c, unsigned char skip)
{
	if (digit >= 1 && digit <= 14 )
	{
		skip |= content[digit-1];
		write_digit(digit, c, 0, skip);
		content[digit-1] |= c & ~skip;
	}
}


void write_seg( unsigned char digit, unsigned char seg)
{
	seg &= SEG_ALL;
	clear_digit(digit, seg);
	set_digit(digit, seg, ~seg);
}

void write_char( unsigned char digit, unsigned char c)
{
	c=char_to_seg(c);
	clear_digit(digit, c);
	set_digit(digit, c, ~c);
}


unsigned char write_str( char* str, unsigned char start)
{
	unsigned char c;
	while (*str)
	{
		c=char_to_seg(*str++);
		clear_digit(start, c);
		set_digit(start++, c, ~c);
		if (start > 14)
			start = 1;
	}
	return start;
}

unsigned char write_str_roll( char* str, unsigned char start, unsigned char end, unsigned char pos)
{
	unsigned char local_pos=pos++;
	unsigned char c;
	for (;start <= end; start++, local_pos++)
	{
		if (*(str+local_pos) == 0)
		{
			local_pos=0;
		}
		c = char_to_seg(*(str+local_pos));
		clear_digit(start, c);
		set_digit(start,c,~c);
	}
	if (*(str+pos) == 0)
		pos = 0;
	return pos;
}

unsigned char write_seg_roll( char* str, unsigned char start, unsigned char end, unsigned char pos)
{
	unsigned char local_pos=pos++;
	unsigned char c;
	for (;start <= end; start++, local_pos++)
	{
		if (*(str+local_pos) == 0)
		{
			local_pos=0;
		}
		c = *(str+local_pos);
		clear_digit(start, c);
		set_digit(start,c,~c);
	}
	if (*(str+pos) == 0)
		pos = 0;
	return pos;
}

unsigned char write_str_blend(char* str, unsigned char start)
{
	static const unsigned char bit_count[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	unsigned char diff[14];

	unsigned char seg_num;
	unsigned char seg,digit;
	unsigned char str_len, seg_count;

	/* empty str */
	if (*str == 0)
		return 0;

	seg_count = 0;
	str_len = 0;

	for (str_len=0;*str;str++)
	{
		diff[str_len] = char_to_seg(*str) ^ content[start+str_len-1];
		seg_count += bit_count[diff[str_len]&0x0f];
		seg_count += bit_count[diff[str_len]>>4];
		str_len++;
	}

	if (seg_count == 0)
		return 0;

	seg_num = rand();

	if (seg_count > 1)
		seg_num %= seg_count;
	else
		seg_num = 0;

	for (digit=0;digit<str_len;digit++)
	{
		if (diff[digit] != 0)
		{
			for (seg=0x01; seg; seg<<=1)
			{
				if (diff[digit] & seg)
				{
					if (seg_num == 0)
					{
						/* update this segment */
						write_seg(digit+start,content[digit+start-1]^seg);
						return 1;
					}
					else
					{
						seg_num--;
					}
				}
			}
		}
	}
	return 0;
}

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
