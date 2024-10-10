#ifndef MOTOR_H_
#define MOTOR_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../../periph/GPIO/GPIO.h"

void Motor_init();
void Motor_On();
void Motor_Off();
void Motor_makeHerz(uint16_t herz, uint8_t speed);

#endif /* MOTOR_H_ */