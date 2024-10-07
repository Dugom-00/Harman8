#include "Safty.h"

char buff[20];
uint16_t distance;

char buff[20];
uint16_t distance;

void Fan_init()
{
	LCD_init();
	Buzzer_init();
	Ultrasonic_init();
}


void Fan_safety()
{
	Ultrasonic_trigger();
	
	if(Ultrasonic_getFlag())
	{
		Ultrasonic_clearFlag(); //Flag가 있으면 초기화 함수가 있어야함
		distance = Ultrasonic_getDistance();
		sprintf(buff, "distance : %03dcm",distance);
		LCD_write_stringXY(0,0,buff);
		if(distance < 10)
		{
			Buzzer_soundOff();
		}
		else
		{
			Fan_buzzer2();
		}
		
	}
}

void Fan_buzzer2()
{
	Buzzer_soundOn();
	Buzzer_makeHerz(2043);
	_delay_ms(200);
	TCNT3 = 0;
	Buzzer_soundOff();
}