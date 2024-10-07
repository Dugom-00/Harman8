#include "LCD.h"

uint8_t lcdControlData;

void Gpio_init()
{
	Gpio_initPin(&LCD_CONTROL_DDR,LCD_RS,OUTPUT);
	Gpio_initPin(&LCD_CONTROL_DDR,LCD_RW,OUTPUT);
	Gpio_initPin(&LCD_CONTROL_DDR,LCD_E,OUTPUT);

	Gpio_initPort(&LCD_DATA_DDR, OUTPUT);
}

void LCD_init()
{
	Gpio_init();
	
	_delay_ms(15);
	LCD_writeCmdData(LCD_FUNCTION_SET);    // LCD_Function_Set      0x38 : 2-Line, 5*8 Type
	_delay_ms(5);
	LCD_writeCmdData(LCD_FUNCTION_SET);
	_delay_ms(1);
	LCD_writeCmdData(LCD_FUNCTION_SET);
	LCD_writeCmdData(LCD_FUNCTION_SET);
	LCD_writeCmdData(LCD_DISPLAY_OFF);	   // LCD_Display_Off      0x08
	LCD_writeCmdData(LCD_DISPLAY_CLEAR);   // LCD_Display_Clear   0x01
	LCD_writeCmdData(LCD_ENTRY_MODE_SET);  // LCD_Entry_Mode_Set   0x06
	LCD_writeCmdData(LCD_DISPLAY_ON);	   // LCD_Display_On      0x0C
}

void Gpio_writeControlData(uint8_t data)
{
	//LCD_CONTROL_PORT = data;
	Gpio_writePort(&LCD_CONTROL_PORT, data);
}

void Gpio_writeData(uint8_t data)
{
	//LCD_DATA_PORT = data;
	Gpio_writePort(&LCD_DATA_PORT, data);
}

void LCD_CmdMode()
{
	// RS -> 0
	lcdControlData &= ~(1<<LCD_RS);
	Gpio_writeControlData(lcdControlData);
}

void LCD_CharMode()
{
	// RS -> 1
	lcdControlData |= (1<<LCD_RS);
	Gpio_writeControlData(lcdControlData);
}

void LCD_writeMode()
{
	// RW -> 0
	lcdControlData &= ~(1<<LCD_RW);
	Gpio_writeControlData(lcdControlData);
}

void LCD_enableHigh()
{
	// E -> 1
	lcdControlData |= (1<<LCD_E);
	Gpio_writeControlData(lcdControlData);
	_delay_ms(1);
}

void LCD_enableLow()
{
	// E -> 0
	lcdControlData &= ~(1<<LCD_E);
	Gpio_writeControlData(lcdControlData);
	_delay_ms(1);
}

void LCD_writeByte(uint8_t data)
{
	Gpio_writeData(data);
}

void LCD_writeCmdData(uint8_t data)
{
	// LCD Instruction Mode
	LCD_CmdMode();      // RS -> 0
	LCD_writeMode();    // RW -> 0
	LCD_enableHigh();   // E -> 1
	LCD_writeByte(data);
	LCD_enableLow();    // E -> 0
}

void LCD_writeCharData(uint8_t data)
{
	// LCD Data Mode
	LCD_CharMode();      // RS -> 1
	LCD_writeMode();     // RW -> 0
	LCD_enableHigh();    // E -> 1
	LCD_writeByte(data);
	LCD_enableLow();     // E -> 0
}

void LCD_writeString(char *str)
{
	for(int i=0; str[i]; i++)
	{
		LCD_writeCharData(str[i]);
	}
}

void LCD_gotoXY(uint8_t row, uint8_t col)
{
	col %= 16, row %= 2;
	
	uint8_t lcdregisterAddress = (0x40 * row) + col;
	uint8_t command = 0x80 + lcdregisterAddress;
	LCD_writeCmdData(command);
}

void LCD_writeStringXY(uint8_t row, uint8_t col, char *str)
{
	LCD_gotoXY(row, col);
	LCD_writeString(str);
}

void LCD_displayOff()
{
	LCD_writeCmdData(LCD_DISPLAY_OFF);   // LCD_Display_Off      0x08
}

void LCD_displayOn()
{
	LCD_writeCmdData(LCD_DISPLAY_ON);   // LCD_Display_On      0x0C
}

void LCD_displayClear()
{
	LCD_writeCmdData(LCD_DISPLAY_CLEAR);   // LCD_Display_Clear   0x01
}