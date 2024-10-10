#ifndef DHT11_H_
#define DHT11_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../../periph/GPIO/GPIO.h"

#define DHT11_DDR		DDRA
#define DHT11_PORT		PORTA
#define DHT11_PIN		PINA
#define DHT11_DATA_PIN	7

enum {DHT11_ERROR, DHT11_OK};
	
typedef struct _DHT11_DataType
{
	uint8_t humidInt;
	uint8_t humidDec;
	uint8_t tempInt;
	uint8_t tempDec;
	uint8_t checkSumError;
}DHT11_DataType;

void DHT11_init();
void DHT11_outputMode();
void DHT11_inputMode();
void DHT11_sendLowSignal();
void DHT11_sendHighSignal();
void DHT11_sendStartSignal();
uint8_t DHT11_readPin();
uint8_t DHT11_waitResponse();
uint8_t DHT11_readSensorData(uint8_t *arry);
DHT11_DataType DHT11_readData();	

#endif /* DHT11_H_ */