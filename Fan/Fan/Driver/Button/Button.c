#include "Button.h"

void Button_init(button_t *btn, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum)
{
	btn->DDR = ddr;
	btn->PIN = pin;
	btn->pinNum = pinNum;
	btn->prevState = RELEASED;
	
	Gpio_initPin(btn->DDR, btn->pinNum, INPUT);	// *btn->DDR &= ~(1<<btn->pinNum);
}

uint8_t Button_GetState(button_t *btn)
{
	uint8_t curState = Gpio_readPin(btn->PIN, btn->pinNum);	// uint8_t curState = *btn->PIN & (1<<btn->pinNum);
	
	if((curState == PUSHED) && (btn->prevState == RELEASED))
	{
		_delay_ms(10);
		btn->prevState = PUSHED;
		return ACT_PUSHED;
	}
	else if((curState != PUSHED) && (btn->prevState == PUSHED))
	{
		_delay_ms(10);
		btn->prevState = RELEASED;
		return ACT_RELEASED;
	}
	return ACT_NONE;
}