/*
 * 7seg.h
 *
 *  Created on: 31.10.2012
 *      Author: PC 8
 */

#ifndef _7SEG_H_
#define _7SEG_H_

#define MECH_DELAY	100
#define DATA_DELAY	0.25

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
#define SEG_ALL 0xFE

void Init_7seg();

unsigned char write_seg_roll( char* str, unsigned char start, unsigned char end, unsigned char pos);
unsigned char write_str_roll( char* str, unsigned char start, unsigned char end, unsigned char pos);
unsigned char write_str( char* str, unsigned char start);
void write_char(unsigned char digit, unsigned char c);
void write_seg(unsigned char digit, unsigned char seg);
unsigned char write_str_blend(char* str, unsigned char start);


const unsigned char char_to_seg(unsigned char c);

#endif
