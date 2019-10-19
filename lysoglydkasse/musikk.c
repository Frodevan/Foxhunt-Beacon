/*
 * CFile1.c
 *
 * Created: 30.01.2018 01.03.50
 *  Author: medlem
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "tone.h"

void playertikk();


const uint8_t melodi[] = "    -... . .- -.-. --- -.    ..-. --- -..- .... ..- -. -    ";
						//    B    E A  C    O   N     F    O   X    H    U   N  T (add your callsign to this string!)
const uint8_t pattern[] = "---."; // Repeating pattern

volatile uint8_t playerstep;
volatile uint8_t playing;
volatile uint8_t patternplaying;

volatile uint16_t ptr;
volatile uint16_t cnt;
volatile uint8_t wit;

void setup_musikk()
{
	setup_lyd();
	cli();
	TCCR0A |= (1<<WGM00) | (1<<WGM01);	// Fast PWM
	TCCR0B |= (1<<CS01) | (1<<CS00);	// Clock Source
	TCCR0B &= ~((1<<WGM02) | (1<<FOC0A) | (1<<FOC0B) | (1<<CS02));
	TIMSK0 |= (1<<TOV0);				// Enable Imterrupts
	playerstep = 0;
	playing = 0;
	sei();
}


ISR(TIM0_OVF_vect)  // Trigges ~240Hz
{
	sei();
	if(!((playerstep++)&0x03)) // triggers ~30Hz
	{
		playertikk();
	}
	
}

void stop_musikk()
{
	cli();
	ptr = 0;
	cnt = 0;
	wit = 0;
	playing = 0;
	patternplaying = 0;
	stopTone();
	sei();
}

//
// Starter spilleren fra starten av melodi
//
void restartMusikk()
{
	cli();
	ptr = 0;
	cnt = 0;
	wit = 0;
	playing = 0xFF;
	patternplaying = 0x00;
	sei();
}

void restartPattern()
{
	cli();
	ptr = 0;
	cnt = 0;
	wit = 0;
	playing = 0x00;
	patternplaying = 0xFF;
	sei();
}

uint8_t isMusikkPlaying()
{
	return playing;
}

void playertikk()  // Kalles hver ~60Hz
{
	while(cnt++ >= wit)		// Spiller vi? Er vi klar til neste event?
	{
		if(playing)
		{
			if(isTone())
			{
				wit = 5;
				stopTone();
			}
			else if(melodi[ptr] == ' ')			// tolk og utf�r event
			{
				wit = 16;
				stopTone();
				ptr++;
			}
			else if(melodi[ptr] == '.')
			{
				wit = 8;
				playTone(7, 6);
				ptr++;
			}
			else if(melodi[ptr] == '-')
			{
				wit = 16;
				playTone(7, 6);
				ptr++;
			}
			else
			{
				stopTone();
				playing = 0;
				break;
			}
			cnt = 0;
		}
		if(patternplaying)
		{
			if(isTone())
			{
				wit = 5;
				stopTone();
			}
			else if(pattern[ptr] == ' ')			// tolk og utf�r event
			{
				wit = 16;
				stopTone();
				ptr++;
			}
			else if(pattern[ptr] == '.')
			{
				wit = 8;
				playTone(7, 6);
				ptr++;
			}
			else if(pattern[ptr] == '-')
			{
				wit = 16;
				playTone(0, 7);
				ptr++;
			}
			else
			{
				ptr = 0;
				break;
			}
			cnt = 0;
		}
	}
}