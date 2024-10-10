
#ifndef CARBUZZER_H_
#define CARBUZZER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../Driver/Buzzer/Buzzer.h"
#include "../../Driver/Ultrasonic/Ultrasonic.h"

void Fan_init();
void Fan_safety();
void Fan_buzzer2();

#endif /* Safty */