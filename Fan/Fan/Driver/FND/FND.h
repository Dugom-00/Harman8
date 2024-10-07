#ifndef FND_H_
#define FND_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../../periph/GPIO/GPIO.h"

#define FND_DIGIT_DDR		DDRE
#define FND_DIGIT_DDR2      DDRG
#define FND_DIGIT_PORT      PORTE
#define FND_DIGIT_PORT2     PORTG
#define FND_DATA_DDR		DDRF
#define FND_DATA_PORT		PORTF

#define FND_DIGIT_1         0
#define FND_DIGIT_2         5
#define FND_DIGIT_3         6
#define FND_DIGIT_4         7

#define FND_DP				10

void FND_init();
void FND_dispNum(uint16_t fndNum);
void FND_setFndData(uint16_t data);
void FND_ISR_Process();
void FND_colonOn();
void FND_colonOff();


#endif /* FND_H_ */