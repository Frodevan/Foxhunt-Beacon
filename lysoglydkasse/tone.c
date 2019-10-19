
/*
 * CFile1.c
 *
 * Created: 30.01.2018 00.50.02
 *  Author: medlem
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t soundstep;
volatile uint8_t play;

//
// Bølgelengder for alle semitonene. Shiftes til høyre for oktaver.
//
const uint_fast16_t scale[] = {61156, 57724, 54484, 51426, 48540, 45815, 43244, 40817, 38526, 36364, 34323, 32396};

void setup_lyd()
{
	cli();
	DDRA |= (1<<PINA7);						// Output pin for speaker (for software timer)
	TCCR1A &= ~((1<<WGM11) | (1<<WGM10));	// Clear on Compare mode
	TCCR1B |= (1<<CS11) | (1<<WGM12);		// Prescaler
	TCCR1B &= ~((1<<WGM13));
	TIMSK1 |= (1<<OCIE1A);					// Enable interrupts

	TCNT1 = 0;								//Preset all Values
	OCR1A = 0xFFFF;
	soundstep = 0;
	play = 0;
	sei();	
}

//
// Spill en tone
//
void playTone(uint8_t tone, uint8_t oktav)
{
	OCR1A = scale[tone%12]>>(oktav%10);
	TCNT1 = 0;
	play = 0xFF;
}

//
// Stopp spillende tone
//
void stopTone()
{
	play = 0;
}

//
// Software-toggle av høytaler
//
ISR(TIM1_COMPA_vect)
{
	if(play)
		PORTA |= (((soundstep++)&0x01)<<PINA7);
	PORTA &= ~((soundstep&0x01)<<PINA7);
}

uint8_t isTone()
{
	return play;
}
