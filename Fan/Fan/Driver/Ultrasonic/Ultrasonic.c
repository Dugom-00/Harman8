#include "Ultrasonic.h"

uint8_t ultrasonicDistanceFlag;

void Ultrasonic_init()
{
	ultrasonicDistanceFlag = 0;
	// Pin init
	USONIC_DDR &= ~(1<<USONIC_ECHO);	// echo INPUT
	USONIC_DDR |= (1<<USONIC_TRIG);	// trigger OUTPUT
	
	// INT2
	EICRA |= (1<<ISC21) | (1<<ISC20);	// Rising Edge Interrupt Enable
	EIMSK |= (1<<INT2);
}

void Ultrasonic_trigger()
{
	USONIC_PORT |= (1<<USONIC_TRIG);
	_delay_us(10);
	USONIC_PORT &= ~(1<<USONIC_TRIG);
}

uint16_t Ultrasonic_getDistance()
{
	// 1/64
	return USONIC_TCNT * 0.068;
}

void Ultrasonic_timerStart()
{
	USONIC_TCNT = 0;
	TCCR1B |= (0<<CS12) | (1<<CS11) | (1<<CS10);
}

void Ultrasonic_timerStop()
{
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
}

void Ultrasonic_setFlag()
{
	ultrasonicDistanceFlag = 1;
}

void Ultrasonic_clearFlag()
{
	ultrasonicDistanceFlag = 0;
}

uint8_t Ultrasonic_getFlag()
{
	return ultrasonicDistanceFlag;
}

void Ultrasonic_switchInterrupt(uint8_t swtichState)
{
	if(swtichState == FALLING_EDGE)
	{
		// next FALLING_EDGE
		EICRA &= ~((1<<ISC21) | (1<<ISC20));
		EICRA |= (1<<ISC21) | (0<<ISC20);
	}
	else
	{
		// next RISING_EDGE
		EICRA &= ~((1<<ISC21) | (1<<ISC20));
		EICRA |= (1<<ISC21) | (1<<ISC20);
	}
}

void Ultrasonic_ISR_Process()
{
	if(USONIC_PIN & (1<<USONIC_ECHO))
	{
		Ultrasonic_timerStart();
		Ultrasonic_switchInterrupt(FALLING_EDGE);
	}
	else
	{
		Ultrasonic_timerStop();
		Ultrasonic_setFlag();
		Ultrasonic_switchInterrupt(RISING_EDGE);
	}
}

uint16_t Ultrasonic_run()
{
	uint16_t distance = 0;
	
	Ultrasonic_trigger();
	_delay_ms(1000);
	
	if(Ultrasonic_getFlag())
	{
		Ultrasonic_clearFlag();
		distance = Ultrasonic_getDistance();
	}
	
	return distance;
}