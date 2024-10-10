#include "Buzzer.h"

void Buzzer_init()
{	
	Gpio_initPin(&DDRE, 4, OUTPUT);
	
	// TIM3, Top ICR, DutyCycle OCR3A
	// Prescaler 1/64
	TCCR3B |= (0<<CS32) | (1<<CS31) | (1<<CS30);
	
	//fast PWM mode, TOP : ICR
	TCCR3B |= (1<<WGM33) | (1<<WGM32);
	TCCR3A |= (1<<WGM31) | (0<<WGM30);
}

void Buzzer_soundOn()
{
	// Non-Inverting Mode
	TCCR3A |= (1<<COM3B1) | (0<<COM3B0);
}

void Buzzer_soundOff()
{
	TCCR3A &= ~((1<<COM3B1) | (1<<COM3B0));
}

void Buzzer_makeHerz(uint16_t herz)
{
	if(herz < 100)
	{
		herz = 100;
	}
	else if(herz > 4000)
	{
		herz = 4000;
	}
	else
	{
		ICR3 = (250000/herz) - 1;
		OCR3B = ICR3 / 2;
	}
}
