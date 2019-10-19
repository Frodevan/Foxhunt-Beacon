/*
 * lysoglydkasse.c
 *
 * Created: 28.01.2018 09.57.27
 * Author : medlem
 */ 

#define F_CPU 8000000UL		// Klokkehastighet

//#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "musikk.h"

void delay_sek(uint8_t t)
{
	for(uint8_t i=0; i<t; i++)
	{
		_delay_ms(1000);
	}
}


int main(void)
{
	//
	// Setup
	//
	setup_musikk();
	
	//
	// Do stuff
	//
    while (1) 
    {
		restartMusikk();
		while(isMusikkPlaying());
		restartPattern();
		delay_sek(25);
		restartMusikk();
		while(isMusikkPlaying());
		delay_sek(60);
    }
}