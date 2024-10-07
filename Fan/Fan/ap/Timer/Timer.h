/*
 * timeClock.h
 *
 * Created: 2024-09-25 오후 1:37:43
 *  Author: kccistc
 */ 


#ifndef TIMECLOCK_H_
#define TIMECLOCK_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../../driver/BUTTON/button.h"
#include "../../driver/FND/FND.h"
#include "../../periph/TIM/TIM.h"

enum {NORMAL, MODIFY, DISPLAY};

void timeClock_init();
void timeClock_execute();
void timeClock_incMillisec();
void timeClock_eventCheck();
void timeClock_run();
void timeClock_incMin();
void timeClock_incHour();

void timeClock_inmilsec();
void timeClock_sec();

#endif /* TIMECLOCK_H_ */