#ifndef __7_SEG__
#define __7_SEG__

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

#endif