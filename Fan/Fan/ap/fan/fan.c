#include "fan.h"

button_t btnOnOff, btnNormal, btnTimer, btnMin, btnSec, btnNext;

uint16_t milisec;
uint8_t sec;
uint8_t min;
uint8_t hour;

uint8_t tsec;
uint8_t tmin;

uint8_t state;
uint8_t normalState;

uint8_t timerData;

uint8_t on_sec, on_min, off_sec, off_min;

char buff[20] = {0,};

void fan_init()
{
	Button_init(&btnOnOff, &DDRA, &PINA, 0);
	
	Button_init(&btnTimer, &DDRA, &PINA, 1);
	Button_init(&btnMin, &DDRA, &PINA, 2);
	Button_init(&btnSec, &DDRA, &PINA, 3);
	
	Button_init(&btnNormal, &DDRA, &PINA, 2);
	Button_init(&btnNext, &DDRA, &PINA, 3);
	
	Motor_init();
	LCD_init();
	FND_init();
	Buzzer_init();
	Ultrasonic_init();
	UART0_init();
	
	TIM0_init();
	TIM2_init();
	
	state = OFF_SETTING;
	normalState = MODE1;
	
	milisec = 0;
	sec = 0;
	min = 0;
	hour = 0;

	tsec = 0;
	tmin = 0;
	
	timerData = 0;
	
	on_sec = 0;
	on_min = 0;
	off_sec = 0;
	off_min = 0;

	Motor_Off();
	
	sprintf(buff, "Fan Off");
	LCD_writeStringXY(0, 0, buff);
	sprintf(buff, "2:TIMER|3:NORMAL");
	LCD_writeStringXY(1, 0, buff);
	
	fan_buzzer();
}

void fan_Timer_incMilisec()
{
	milisec = (milisec + 1) % 1000;
	if(milisec) return;

	sec = (sec + 1) % 60;
	if(sec) return;

	min = (min + 1) % 60;
	if (min) return;

	hour = (hour + 1) % 24;
}

void fan_setTimerMin()
{
	milisec = 0;
	sec = 0;
	min = 0;
	hour = 0;
	
	tmin = (tmin + 1)%60;
}

void fan_setTimerSec()
{
	milisec = 0;
	sec = 0;
	min = 0;
	hour = 0;
	
	tsec = (tsec + 1)%60;
}

void fan_normal()
{
	if(normalState == MODE1)
	{
		Motor_On();
		Motor_makeHerz(1000, 4);
	}
	else if(normalState == MODE2)
	{
		Motor_On();
		Motor_makeHerz(1000, 3);
	}
	else if(normalState == MODE3)
	{
		Motor_On();
		Motor_makeHerz(1000, 1);
	}
}

void fan_run()
{
	fan_buttonCtrl();
	
	fan_UARTCtrl();
}

void fan_UARTCtrl()
{
	if(UART0_getRxFlag())
	{
		UART0_clearRxFlag();
		
		uint8_t *rxString = (char *)UART0_getRxBuff();
		
		if(!strcmp((const char *)rxString, "ON\n"))
		{
			fan_buzzer();
			
			fan_normal();
			
			state = OFF_ON;
		}
		else if(!strcmp((const char *)rxString, "OFF\n"))
		{
			fan_buzzer();
			
			Motor_Off();
			
			state = OFF_SETTING;
		}
		else if (!strcmp((const char *)rxString, "FAN_UP\n"))  // 'FAN_UP' 명령 수신 시 팬 속도 증가
		{
			fan_buzzer();
			switch(normalState)
			{
				case MODE1 :
				normalState = MODE2;
				break;

				case MODE2 :
				normalState = MODE3;
				break;

				case MODE3 :
				normalState = MODE1;
				break;
			}
			fan_normal();  // 새로운 모드로 팬 동작 시작
		}
		else if (!strcmp((const char *)rxString, "ON_MIN+\n"))  // 'ON_MIN+' 명령 수신 시 ON 상태에서 타이머 분 증가
		{
			fan_buzzer();
			
			on_min = (on_min + 1) % 60;  // ON 상태 타이머 분 증가

			LCD_displayClear();
			sprintf(buff, "ON Timer: %02d:%02d", on_min, on_sec);
			LCD_writeStringXY(0, 0, buff);
		}
		else if (!strcmp((const char *)rxString, "ON_SEC+\n"))  // 'ON_SEC+' 명령 수신 시 ON 상태에서 타이머 초 증가
		{
			fan_buzzer();
			
			on_sec = (on_sec + 1) % 60;  // ON 상태 타이머 초 증가
			
			LCD_displayClear();
			sprintf(buff, "ON Timer: %02d:%02d", on_min, on_sec);
			LCD_writeStringXY(0, 0, buff);
			
		}
		else if (!strcmp((const char *)rxString, "OFF_MIN+\n"))  // 'OFF_MIN+' 명령 수신 시 OFF 상태에서 타이머 분 증가
		{
			fan_buzzer();
			
			off_min = (off_min + 1) % 60;  // OFF 상태 타이머 분 증가
			
			LCD_displayClear();
			sprintf(buff, "OFF Timer: %02d:%02d", off_min, off_sec);
			LCD_writeStringXY(0, 0, buff);
		}
		else if (!strcmp((const char *)rxString, "OFF_SEC+\n"))  // 'OFF_SEC+' 명령 수신 시 OFF 상태에서 타이머 초 증가
		{
			fan_buzzer();
			
			off_sec = (off_sec + 1) % 60;  // OFF 상태 타이머 초 증가
			
			LCD_displayClear();
			sprintf(buff, "OFF Timer: %02d:%02d", off_min, off_sec);
			LCD_writeStringXY(0, 0, buff);
		}
		else if (!strcmp((const char *)rxString, "Clock_Set\n"))  // 'Clock_Set' 명령 수신 시 카운트다운 시작
		{
			Timer_reset();
			
			fan_buzzer();
			
			LCD_displayClear();
			sprintf(buff, "Countdown Started");
			LCD_writeStringXY(0, 0, buff);
			LCD_writeStringXY(1, 0, "Timing...");
			// 추가적으로 카운트다운 시작 로직을 여기에 작성
		}
	}
	
	if(((on_sec) || (on_min)) && (sec == on_sec) && (min == on_min))
	{
		fan_buzzer();
		
		fan_normal();
		
		on_sec = 0;
		on_min = 0;
	}
	if(((off_sec) || (off_min)) && (sec == off_sec) && (min == off_min))
	{
		fan_buzzer();
		
		Motor_Off();
		
		off_sec = 0;
		off_min = 0;
	}
}

void fan_buttonCtrl()
{
	switch(state)
	{
		case OFF_SETTING :
		if(Button_GetState(&btnOnOff) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			LCD_displayClear();
			sprintf(buff, "Fan On");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "2:TIMER|3:NORMAL");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_ON;
		}
		else if(Button_GetState(&btnTimer) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "3:MIN|4:SEC");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_TIMER;
			
			MIN_SEC_RESET();
		}
		else if(Button_GetState(&btnNormal) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			LCD_displayClear();
			sprintf(buff, "Fan Speed");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "4:NEXT");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_NORMAL;
		}
		else
		{
			TimerData_Set();
			FND_setFndData(timerData);
		}
		break;
		
		case OFF_TIMER :
		if(Button_GetState(&btnOnOff) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			Timer_reset();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "%02d:%02d", tmin, tsec);
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_ON;
		}
		else if(Button_GetState(&btnTimer) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			LCD_displayClear();
			sprintf(buff, "Fan Off");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "2:TIMER|3:NORMAL");
			LCD_writeStringXY(1, 0, buff);
			
			MIN_SEC_RESET();
			
			state = OFF_SETTING;
		}
		else if(Button_GetState(&btnMin) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			fan_setTimerMin();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "%02d:%02d", tmin, tsec);
			LCD_writeStringXY(1, 0, buff);
		}
		else if(Button_GetState(&btnSec) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			fan_setTimerSec();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "%02d:%02d", tmin, tsec);
			LCD_writeStringXY(1, 0, buff);
		}
		else
		{
			if((min == tmin) && (sec == tsec))
			{
				fan_buzzer();
				
				fan_normal();
				
				LCD_displayClear();
				sprintf(buff, "Fan On");
				LCD_writeStringXY(0, 0, buff);
				sprintf(buff, "2:TIMER|3:NORMAL");
				LCD_writeStringXY(1, 0, buff);
				
				state = OFF_ON;
			}
			timerData = ((min%60)*100) + (sec%60);
			FND_setFndData(timerData);
		}
		break;
		
		case OFF_NORMAL :
		if(Button_GetState(&btnOnOff) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			LCD_displayClear();
			sprintf(buff, "Fan On");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "%d", normalState);
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_ON;
		}
		else if(Button_GetState(&btnNormal) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			LCD_displayClear();
			sprintf(buff, "Fan Off");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "2:TIMER|3:NORMAL");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_SETTING;
		}
		else if(Button_GetState(&btnNext) == ACT_RELEASED)
		{
			switch(normalState)
			{
				case MODE1 :
				normalState = MODE2;
				fan_Motor_Off();
				break;
				
				case MODE2 :
				normalState = MODE3;
				fan_Motor_Off();
				break;
				
				case MODE3 :
				normalState = MODE1;
				fan_Motor_Off();
				break;
			}
		}
		else
		{
			TimerData_Set();
			FND_setFndData(timerData);
		}
		break;
		
		case OFF_ON :
		if(Button_GetState(&btnOnOff) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			LCD_displayClear();
			sprintf(buff, "Fan Off");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "2:TIMER|3:NORMAL");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_SETTING;
		}
		else if(Button_GetState(&btnTimer) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "3:MIN|4:SEC");
			LCD_writeStringXY(1, 0, buff);
			
			state = ON_TIMER;
			
			MIN_SEC_RESET();
		}
		else if(Button_GetState(&btnNormal) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			LCD_displayClear();
			sprintf(buff, "Fan Speed");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "4:NEXT");
			LCD_writeStringXY(1, 0, buff);
			
			state = ON_NORMAL;
		}
		else
		{
			TimerData_Set();
			FND_setFndData(timerData);
			Motor_Safety();
		}
		break;
		
		case ON_TIMER :
		if(Button_GetState(&btnOnOff) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			Timer_reset();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "%02d:%02d", tmin, tsec);
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_SETTING;
		}
		else if(Button_GetState(&btnTimer) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			LCD_displayClear();
			sprintf(buff, "Fan On");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "2:TIMER|3:NORMAL");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_ON;
			
			MIN_SEC_RESET();
		}
		else if(Button_GetState(&btnMin) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			fan_setTimerMin();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "%02d:%02d", tmin, tsec);
			LCD_writeStringXY(1, 0, buff);
		}
		else if(Button_GetState(&btnSec) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			fan_setTimerSec();
			
			LCD_displayClear();
			sprintf(buff, "Timer Mode");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "%02d:%02d", tmin, tsec);
			LCD_writeStringXY(1, 0, buff);
		}
		else
		{
			if((min == tmin) && (sec == tsec))
			{
				Motor_Off();
				
				LCD_displayClear();
				sprintf(buff, "Fan Off");
				LCD_writeStringXY(0, 0, buff);
				sprintf(buff, "2:TIMER|3:NORMAL");
				LCD_writeStringXY(1, 0, buff);
				
				state = OFF_SETTING;
			}
			
			timerData = ((min%60)*100) + (sec%60);
			FND_setFndData(timerData);
			Motor_Safety();
		}
		break;
		
		case ON_NORMAL :
		if(Button_GetState(&btnOnOff) == ACT_RELEASED)
		{
			fan_buzzer();
			Motor_Off();
			
			LCD_displayClear();
			sprintf(buff, "Fan Off");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "2:TIMER|3:NORMAL");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_SETTING;
		}
		else if(Button_GetState(&btnNormal) == ACT_RELEASED)
		{
			fan_buzzer();
			fan_normal();
			
			LCD_displayClear();
			sprintf(buff, "Fan On");
			LCD_writeStringXY(0, 0, buff);
			sprintf(buff, "2:TIMER|3:NORMAL");
			LCD_writeStringXY(1, 0, buff);
			
			state = OFF_ON;
		}
		else if(Button_GetState(&btnNext) == ACT_RELEASED)
		{
			switch(normalState)
			{
				case MODE1 :
				normalState = MODE2;
				fan_Motor_On();
				break;
				
				case MODE2 :
				normalState = MODE3;
				fan_Motor_On();
				break;
				
				case MODE3 :
				normalState = MODE1;
				fan_Motor_On();
				break;
			}
		}
		else
		{
			timerData = 0;
			FND_setFndData(timerData);
			Motor_Safety();
		}
		break;
	}
}

void Motor_Safety()
{
	uint16_t distance;
	Ultrasonic_trigger();
	_delay_ms(1);
	
	if(Ultrasonic_getFlag())
	{
		Ultrasonic_clearFlag();
		distance = Ultrasonic_getDistance();
		if(distance <= 10)
		{
			Motor_Off();
		}
		else if((min == tmin) && (sec == tsec))
		{
			Motor_Off();
		}
		else
		{
			fan_normal();
		}
	}
}

void fan_buzzer()
{
	Buzzer_soundOn();
	Buzzer_makeHerz(1000);
	_delay_ms(200);
	TCNT3 = 0;
	Buzzer_soundOff();
}

void fan_Ultrasonic_ISR_Process()
{
	Ultrasonic_ISR_Process();
}

void fan_FND_ISR_Process()
{
	FND_ISR_Process();
}

void fan_UART0_ISR_Process()
{
	UART0_ISR_Process();
}

void fan_Motor_On()
{
	fan_buzzer();
	fan_normal();
	LCD_displayClear();
	sprintf(buff, "Fan Speed");
	LCD_writeStringXY(0, 0, buff);
	sprintf(buff, "%d", normalState);
	LCD_writeStringXY(1, 0, buff);
}

void fan_Motor_Off()
{
	fan_buzzer();
	Motor_Off();
	LCD_displayClear();
	sprintf(buff, "Fan Speed");
	LCD_writeStringXY(0, 0, buff);
	sprintf(buff, "%d", normalState);
	LCD_writeStringXY(1, 0, buff);
}

void Timer_reset()
{
	milisec = 0;
	sec = 0;
	min = 0;
	hour = 0;

}

void MIN_SEC_RESET()
{
	tmin = 0;
	tsec = 0;
}

void TimerData_Set()
{
	timerData = 0;
}
