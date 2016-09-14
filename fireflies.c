#include <avr/io.h>
#include <util/delay.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <inttypes.h>
#include <avr/interrupt.h>
//#include <string.h>

#define	FADESTEPS	20

uint8_t step = 0;
uint8_t seq[][16] = {
	{ 15,  16,  17,  18,  20,  30,  35, 100, 150, 155, 200, 201, 210, 211,   0,   0},
	{  1,   2,   3,  21,  28,  29,  50,  60, 110, 130, 190, 195, 250, 251, 252, 253},
	{ 20,  21,  50,  51,  63,  70, 105, 106, 170, 180, 240, 242, 243, 247,   0,   0},
	{ 41,  45,  80,  82, 100, 101, 110, 115, 189, 190, 195, 200, 233, 235,   0,   0},
	{127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142},
	{ 97, 100, 117, 150, 177, 180, 209, 215, 220, 230, 248, 249,   0,   0,   0,   0},
	{ 33,  40,  70,  99, 119, 120, 121, 122, 124, 134, 168, 192, 193, 194,   0,   0},
	{  5,   6,  68,  69, 105, 108, 146, 156, 154, 159, 162, 166, 180, 188, 244, 248}
};

ISR(TIMER0_OVF_vect)
{
	uint8_t i,j;
	uint8_t fadein = 0;
	uint8_t fadeout = 0;

	/* figure out fading fireflies for current step */
	for (j=0; j<8; j++)
		for (i=0; seq[j][i]!=0 ;i++)
			if (step == seq[j][i])
			{
				if (PORTB & (1<<j))
					fadeout |= (1<<j);
				else
					fadein |= (1<<j);
				break;
			}

	/* do the fading */
	for(i=0; i<FADESTEPS; i++)
	{
		PORTB = (PORTB | fadein) & ~fadeout;
		_delay_ms(i);
		PORTB = (PORTB & ~fadein) | fadeout;
		_delay_ms(FADESTEPS-i);
		PORTB = (PORTB | fadein) & ~fadeout;
	}

	step++;
}

int main (void)
{
	TIMSK = (1<<TOIE0);
	TCCR0 = (1<<CS02) | (0<<CS01) | (1<<CS00);
	sei();
	DDRB = 0xFF;
	while(1);
	return 0;
}

