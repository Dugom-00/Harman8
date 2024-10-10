#include "TIM.h"

// 1ms Timer/Counter Overflow Interrupt init
void TIM0_init()
{
	TCCR0 |= ((1<<CS02) | (0<<CS01) | (1<<CS00));
	TIMSK |= (1<<TOIE0);
	
	TCNT0 = 130;
}

// 1ms Timer/Counter Output Compare Match Interrupt init
void TIM2_init()
{
	// PreScaler / 64
	TCCR2 |= (0<<CS22) | (1<<CS21) | (1<<CS20);
	// CTC mode Enable
	TCCR2 |= (1<<WGM21) | (0<<WGM20);
	// TIMSK Compare Match Interrupt Enable
	TIMSK |= (1<<OCIE2);
	OCR2 = 250 - 1;
}
