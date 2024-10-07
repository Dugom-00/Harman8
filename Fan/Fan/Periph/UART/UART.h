#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void UART0_init();
void UART0_Transmit( unsigned char data );
unsigned char UART_Receive( void );
void UART0_printString(char *str);
uint8_t UART0_avail();
void UART0_ISR_Process();
uint8_t UART0_getRxFlag();
void UART0_clearRxFlag();
void UART0_setRxFlag();
uint8_t* UART0_getRxBuff();


#endif /* UART_H_ */