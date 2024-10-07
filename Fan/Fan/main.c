#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ap/fan/fan.h"

ISR(TIMER0_OVF_vect)
{
	fan_FND_ISR_Process();
	TCNT0 = 130;
}

ISR(INT2_vect)
{
	fan_Ultrasonic_ISR_Process();
}

ISR(TIMER2_COMP_vect)
{
	fan_Timer_incMilisec();
}

ISR(USART0_RX_vect)
{
	fan_UART0_ISR_Process();
}

int main(void)
{
	fan_init();

	sei();

	while (1)
	{
		fan_run();
	}
}
