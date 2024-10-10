#ifndef FAN_H_
#define FAN_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "../../driver/Ultrasonic/Ultrasonic.h"
#include "../../driver/FND/FND.h"
#include "../../driver/Motor/Motor.h"
#include "../../driver/Button/Button.h"
#include "../../driver/Buzzer/Buzzer.h"
#include "../../driver/LCD/LCD.h"
#include "../../periph/TIM/TIM.h"
#include "../../periph/UART/UART.h"
#include "../../common/timeTick/timeTick.h"

enum {OFF_SETTING, OFF_TIMER, OFF_NORMAL, OFF_ON, ON_TIMER, ON_NORMAL};
enum {MODE1, MODE2, MODE3, AUTO};

void fan_init();
void fan_Timer_incMilisec();
void fan_setTimerMin();
void fan_setTimerSec();
void fan_normal();
void fan_run();
void fan_UARTCtrl();
void fan_buttonCtrl();
void Motor_Safety();
void fan_buzzer();
void fan_Ultrasonic_ISR_Process();
void fan_FND_ISR_Process();
void fan_UART0_ISR_Process();
void Timer_reset();
void MIN_SEC_RESET();
void TimerData_Set();
void fan_Motor_On();
void fan_Motor_Off();

#endif /* FAN_H_ */