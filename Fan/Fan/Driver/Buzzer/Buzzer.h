#ifndef BUZZER_H_
#define BUZZER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../../periph/GPIO/GPIO.h"

void Buzzer_init();
void Buzzer_soundOn();
void Buzzer_soundOff();
void Buzzer_makeHerz(uint16_t herz);


#endif /* BUZZER_H_ */