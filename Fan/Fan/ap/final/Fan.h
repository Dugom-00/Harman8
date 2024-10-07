#ifndef FAN_H_
#define FAN_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../../Driver/Button/Button.h"
#include "../../Driver/LCD/LCD.h"
#include "../../Driver/FND/FND.h"
#include "../../Driver/DHT11/DHT11.h"
#include "../../Driver/Buzzer/Buzzer.h"
#include "../../Driver/Ultrasonic/Ultrasonic.h"
#include "../../Periph/GPIO/GPIO.h"
#include "../../Periph/TIM/TIM.h"


// Function declarations
void init_peripherals();
void update_lcd_display();
void handle_btn1();
void handle_btn2();
void handle_btn3();
void handle_btn4();
void handle_ultrasonic();

#endif /* FAN_H_ */