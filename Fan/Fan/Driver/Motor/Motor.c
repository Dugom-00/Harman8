#include "Motor.h"

void Motor_init()
{
	Gpio_initPin(&DDRE, 3, OUTPUT);
	
	// TIM3, Top ICR, DutyCycle OCR3A
	// Prescaler 1/64
	TCCR3B |= (0<<CS32) | (1<<CS31) | (1<<CS30);
	
	//fast PWM mode, TOP : ICR
	TCCR3B |= (1<<WGM33) | (1<<WGM32);
	TCCR3A |= (1<<WGM31) | (0<<WGM30);
}

void Motor_On()
{
	// Non-Inverting Mode
	TCCR3A |= (1<<COM3A1) | (0<<COM3A0);
}

void Motor_Off()
{
	// 모터 PWM 비활성화
	TCCR3A &= ~((1<<COM3A1) | (1<<COM3A0));
}

void Motor_makeHerz(uint16_t herz, uint8_t speed)
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
		OCR3A = ICR3 / speed;
	}
}