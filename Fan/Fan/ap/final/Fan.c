#include "Fan.h"
#include "../../Periph/GPIO/GPIO.h"
#include "../../Driver//Button/Button.h"
#include "../../Driver/LCD/LCD.h"
#include "../../Driver/FND/FND.h"
#include "../../Driver/DHT11/DHT11.h"
#include "../../Driver/Buzzer/Buzzer.h"
#include "../../Driver/Ultrasonic/Ultrasonic.h"
#include "../../Periph/TIM/TIM.h"

// 버튼, LCD, FND, 초음파 센서 등의 상태 변수
uint8_t btn1State = 0;  // on/off
uint8_t btn2State = 0;  // timer mode
uint8_t btn3State = 0;  // speed control mode
uint8_t btn4State = 0;  // auto control mode (DHT11)
uint8_t timerMode = 0;  // Timer setting (0: Off, 1: Set Timer On, 2: Set Timer Off)
uint8_t speedLevel = 0; // 0: Off, 1: Low, 2: Medium, 3: High
uint8_t naturalWind = 0;

void init_peripherals() {
	// 버튼 입력 설정
	Gpio_initPin(&DDRA, PA0, input);
	Gpio_initPin(&DDRA, PA1, input);
	Gpio_initPin(&DDRA, PA2, input);
	Gpio_initPin(&DDRA, PA3, input);
	Gpio_initPin(&DDRA, PA7, input); // 온습도 센서 입력용 (PA7)

	// LCD 제어 핀 설정
	Gpio_initPin(&DDRB, PB5, output); // RS
	Gpio_initPin(&DDRB, PB6, output); // RW
	Gpio_initPin(&DDRB, PB7, output); // E

	// LCD 데이터 핀 설정
	Gpio_initPort(&DDRC, output); // D0~D7

	// 초음파 센서 핀 설정
	Gpio_initPin(&DDRD, PD2, output); // TRIG
	Gpio_initPin(&DDRD, PD3, input);  // ECHO

	// 전동 모터 및 FND 디지트 출력 설정
	Gpio_initPin(&DDRE, PE3, output); // 전동 모터
	Gpio_initPin(&DDRE, PE4, output); // FND D1
	Gpio_initPin(&DDRE, PE5, output); // FND D2
	Gpio_initPin(&DDRE, PE6, output); // FND D3
	Gpio_initPin(&DDRE, PE7, output); // FND D4

	// FND 세그먼트 출력 설정
	Gpio_initPort(&DDRF, output); // a~g, DE

	// 모듈 초기화
	LCD_init();
	_delay_ms(50);  // LCD 초기화 대기
	FND_init();
	DHT11_init();
	Buzzer_init();
	Ultrasonic_init();
	TIM0_init();
	TIM2_init();

	// 인터럽트 활성화
	sei();

	// 초기 LCD 디스플레이 업데이트
	update_lcd_display();
}

void update_lcd_display() {
	LCD_display_clear();
	_delay_ms(5); // LCD 업데이트 대기
	LCD_write_stringXY(0, 0, "1:onoff 2:timer");
	LCD_write_stringXY(1, 0, "3:spd 4:auto");
}

void handle_btn1() {
	if (btn1State) {
		speedLevel = 1; // 기본 팬 속도를 Low로 설정
		LCD_write_stringXY(0, 0, "Fan ON ");
		FND_setFndNum(speedLevel);
		Buzzer_soundOn();
		_delay_ms(100);
		Buzzer_soundOff();
		} else {
		speedLevel = 0; // 팬 끄기
		LCD_write_stringXY(0, 0, "Fan OFF");
		FND_setFndNum(speedLevel);
	}
}

void handle_btn2() {
	if (timerMode == 1) {
		LCD_write_stringXY(1, 0, "Set Timer ON ");
		} else if (timerMode == 2) {
		LCD_write_stringXY(1, 0, "Set Timer OFF");
	}
}

void handle_btn3() {
	speedLevel = (speedLevel + 1) % 4; // 팬 속도 조절 (OFF, Low, Medium, High)
	if (speedLevel == 0) {
		LCD_write_stringXY(0, 0, "Fan OFF  ");
		} else if (speedLevel == 1) {
		LCD_write_stringXY(0, 0, "Speed: Low ");
		} else if (speedLevel == 2) {
		LCD_write_stringXY(0, 0, "Speed: Medium");
		} else if (speedLevel == 3) {
		LCD_write_stringXY(0, 0, "Speed: High ");
	}
	FND_setFndNum(speedLevel);
	Buzzer_soundOn();
	_delay_ms(50);
	Buzzer_soundOff();
}

void handle_btn4() {
	naturalWind = !naturalWind;
	if (naturalWind) {
		LCD_write_stringXY(1, 0, "Auto: On ");
		DHT11_DataType dhtData = DHT11_readData();
		char tempStr[16];
		sprintf(tempStr, "Temp: %dC", dhtData.tempInt);
		LCD_write_stringXY(1, 8, tempStr);
		} else {
		LCD_write_stringXY(1, 0, "Auto: Off");
	}
}

void handle_ultrasonic() {
	uint16_t distance = Ultrasonic_getDistance();
	if (distance < 30) {
		Buzzer_soundOn();
		_delay_ms(100);
		Buzzer_soundOff();
	}
	if (distance < 10) {
		btn1State = 0; // 팬 끄기 (너무 가까울 때)
		handle_btn1();
	}
}

// ISR 핸들러 (버튼 인터럽트)
ISR(INT0_vect) {
	btn1State = !btn1State;
	handle_btn1();
}

ISR(INT1_vect) {
	timerMode = (timerMode + 1) % 3;
	handle_btn2();
}

ISR(INT2_vect) {
	handle_btn3();
}

ISR(INT3_vect) {
	handle_btn4();
}
