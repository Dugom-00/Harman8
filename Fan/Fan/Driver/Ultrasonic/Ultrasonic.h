#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define USONIC_DDR		DDRD
#define USONIC_PORT		PORTD
#define USONIC_PIN		PIND
#define USONIC_TRIG		3
#define USONIC_ECHO		2
#define USONIC_TCNT		TCNT1

enum {FALLING_EDGE, RISING_EDGE};
	
void Ultrasonic_timerStart();
void Ultrasonic_timerStop();
void Ultrasonic_setFlag();
void Ultrasonic_clearFlag();
uint8_t Ultrasonic_getFlag();
void Ultrasonic_switchInterrupt(uint8_t swtichState);
void Ultrasonic_init();
void Ultrasonic_trigger();
uint16_t Ultrasonic_getDistance();
void Ultrasonic_ISR_Process();
uint16_t Ultrasonic_run();

#endif /* ULTRASONIC_H_ */