#include "UART.h"

uint8_t uart0RxFlag = 0;
uint8_t uart0RxBuff[100];

void UART0_init()
{
	// UART0, TxD, RxD, x2 Mode, 9600 Baud Rate
	// 8bit Data bit, parity bit none, stop 1bit
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0A |= (1<<U2X0);
	UCSR0B |= (1<<RXCIE0);
	UBRR0L = 207;
}

void UART0_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char UART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void UART0_printString(char *str)
{
	for(int i = 0; str[i]; i++)
	{
		UART0_Transmit(str[i]);
	}
}

uint8_t UART0_avail()
{
	if(!(UCSR0A & (1<<RXC0)))
	{
		return 0;		// Rx data X
	}
	return 1;			// Rx data O
}

void UART0_ISR_Process()
{
	static uint8_t uart0RxTail = 0;
	
	uint8_t rx0Data = UDR0;
	
	if(rx0Data == '\n')
	{
		uart0RxBuff[uart0RxTail] = rx0Data;
		uart0RxTail++;
		uart0RxBuff[uart0RxTail] = 0;
		uart0RxTail = 0;
		uart0RxFlag = 1;
	}
	else
	{
		uart0RxBuff[uart0RxTail] = rx0Data;
		uart0RxTail++;
	}
}

uint8_t UART0_getRxFlag()
{
	return uart0RxFlag;
}

void UART0_clearRxFlag()
{
	uart0RxFlag = 0;
}

void UART0_setRxFlag()
{
	uart0RxFlag = 1;
}

uint8_t* UART0_getRxBuff()
{
	return uart0RxBuff;
}