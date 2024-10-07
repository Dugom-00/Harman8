/*
* timeClock.c
*
* Created: 2024-09-25 오후 1:37:35
*  Author: kccistc
*/
#include "Timer.h"

uint16_t millisec;
uint8_t sec, min, hour;

uint8_t timeClockState;

button_t btnModi, btnNormal, btnHour, btnMin;

void timeClock_init(){
	millisec = 0;
	sec = 19, min = 27, hour =13;
	timeClockState = NORMAL;
	
	FND_init();
	TIM0_init();
	TIM2_init();
	
	Button_init(&btnModi, &DDRA, &PINA, 0);
	Button_init(&btnNormal, &DDRA, &PINA, 1);
	Button_init(&btnHour, &DDRA, &PINA, 2);
	Button_init(&btnMin, &DDRA, &PINA, 3);
}

void timeClock_execute(){
	timeClock_eventCheck();
	timeClock_run();
}

void timeClock_incMillisec(){
	millisec = (millisec + 1)%1000;
	if(timeClockState == NORMAL)
	{
		if (millisec < 500) FND_colonOn();
		else FND_colonOff();
	}
	else
	if(timeClockState == DISPLAY)
	{/*
		millisec = (millisec + 1)%1000;*/
		if (millisec%100 < 50) FND_colonOn();
		else FND_colonOff();
	}

	if (millisec) return;

	sec = (sec + 1)%60;
	if (sec) return;

	min = (min + 1)%60;
	if (min) return;

	hour = (hour + 1) % 24;
}

void timeClock_eventCheck(){
	switch (timeClockState)
	{
		case MODIFY:
		if (Button_GetState(&btnNormal) == ACT_RELEASED){
			timeClockState = NORMAL;
		}
		if (Button_GetState(&btnHour) == ACT_RELEASED){
			timeClock_incHour();
		}
		if (Button_GetState(&btnMin) == ACT_RELEASED){
			timeClock_incMin();
		}
		break;
		
		case NORMAL:
		if (Button_GetState(&btnModi) == ACT_RELEASED){
			timeClockState = MODIFY;
		}
		else if (Button_GetState(&btnNormal)==ACT_RELEASED)
		{
			timeClockState = DISPLAY;
		}
		break;
		
		case DISPLAY :
		if (Button_GetState(&btnNormal)==ACT_RELEASED)
		{
			timeClockState = NORMAL;
		}
		break;
	}
}

void timeClock_run()
{
	uint16_t timeClockData;
	if(timeClockState == DISPLAY)
	{
		timeClockData = (sec * 100) + millisec / 10;
	}
	else 
	{
		timeClockData = (hour * 100) + min;
	}
	
	FND_setFndNum(timeClockData);
}

void timeClock_incMin()
{
	min = (min + 1) % 60;
	if (min)return;
	hour++;
}

void timeClock_incHour()
{
	hour = (hour + 1) % 24;
}