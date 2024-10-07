#include "Fan_Button.h"

uint8_t fanState;
uint8_t fanMode;
uint8_t timerMinutes = 0;
uint8_t timerSeconds = 0;
uint8_t isTimerMode = 0;  // 타이머 모드 여부 플래그
button_t btnOnOff, btnTimer, btnSpeed, btnAuto;

void Fan_Button_Init()
{
	fanState = FAN_STATE_OFF;
	fanMode = FAN_MODE1;  // 기본 속도를 MODE1으로 설정

	Button_init(&btnOnOff, &DDRA, &PINA, 0);  // On/Off 버튼 (PA0)
	Button_init(&btnTimer, &DDRA, &PINA, 1);  // 타이머 버튼 (PA1)
	Button_init(&btnSpeed, &DDRA, &PINA, 2);  // 속도 버튼 (PA2)
	Button_init(&btnAuto, &DDRA, &PINA, 3);   // 자동 모드 버튼 (PA3)

	Gpio_initPin(&DDRE, 3, output);  // PE3을 출력으로 설정

	LCD_init();
	FND_init();  // FND 초기화
}

void Fan_Button_Execute() {
	Fan_EventCheck();
	Fan_Run();
}

void Fan_EventCheck()
{
	switch (fanState) {
		case FAN_STATE_OFF:
		if (Button_GetState(&btnOnOff) == ACT_RELEASED) {
			fanState = FAN_STATE_ON; // ON 모드로 전환
			LCD_display_clear();
			LCD_write_stringXY(0, 0, "Fan ON");
		}
		break;

		case FAN_STATE_ON:
		if (Button_GetState(&btnOnOff) == ACT_RELEASED)
		{
			fanState = FAN_STATE_OFF; // OFF 모드로 전환
			Gpio_writepin(&PORTE, 3, Gpio_pinreset); // 팬 끄기
			LCD_display_clear(); // LCD 클리어
			LCD_write_stringXY(0, 0, "Fan OFF");
		}
		
		else if (Button_GetState(&btnSpeed) == ACT_RELEASED)
		{
			if (!isTimerMode) {
				if (fanMode == FAN_MODE1) {
					fanMode = FAN_MODE2;
				}
				else if (fanMode == FAN_MODE2) {
					fanMode = FAN_MODE3;
				}
				else {
					fanMode = FAN_MODE1;
				}

				// 모드 변경 시 LCD 업데이트
				LCD_display_clear();
				LCD_write_stringXY(0, 0, "Fan ON");
				switch (fanMode) {
					case FAN_MODE1:
					LCD_write_stringXY(1, 0, "MODE1");
					break;
					case FAN_MODE2:
					LCD_write_stringXY(1, 0, "MODE2");
					break;
					case FAN_MODE3:
					LCD_write_stringXY(1, 0, "MODE3");
					break;
				}
			}
		}
		else if (Button_GetState(&btnTimer) == ACT_RELEASED)
		{
			isTimerMode = !isTimerMode;  // 타이머 모드 전환
			if (isTimerMode)
			{
				LCD_display_clear();
				LCD_write_stringXY(0, 0, "Timer Set:");
				char buffer[16];
				sprintf(buffer, "%02d:%02d", timerMinutes, timerSeconds);
				LCD_write_stringXY(1, 0, buffer);
				FND_setFndNum((timerMinutes * 100) + timerSeconds); // FND에 타이머 값 출력
			}
			else if (Button_GetState(&btnOnOff) == ACT_RELEASED)
			{
				fanState = FAN_STATE_OFF; // OFF 모드로 전환
				Gpio_writepin(&PORTE, 3, Gpio_pinreset); // 팬 끄기
				LCD_display_clear(); // LCD 클리어
				LCD_write_stringXY(0, 0, "Fan OFF");
			}
			
			else
			{
				LCD_display_clear();
				LCD_write_stringXY(0, 0, "Fan ON");
				switch (fanMode) {
					case FAN_MODE1:
					LCD_write_stringXY(1, 0, "MODE1");
					break;
					case FAN_MODE2:
					LCD_write_stringXY(1, 0, "MODE2");
					break;
					case FAN_MODE3:
					LCD_write_stringXY(1, 0, "MODE3");
					break;
				}
			}
		}
		break;

		case FAN_STATE_TIMER_MODE:
		// 타이머 모드에서 3번, 4번 버튼으로 분과 초 조정
		if (Button_GetState(&btnSpeed) == ACT_RELEASED)
		{
			timerMinutes = (timerMinutes + 1) % 60;  // 분 증가
			char buffer[16];
			sprintf(buffer, "%02d:%02d", timerMinutes, timerSeconds);
			LCD_write_stringXY(1, 0, buffer);  // LCD에 현재 시간 표시
			FND_setFndNum((timerMinutes * 100) + timerSeconds); // FND에 시간 표시
		}
		
		if (Button_GetState(&btnAuto) == ACT_RELEASED) {
			timerSeconds = (timerSeconds + 1) % 60;  // 초 증가
			char buffer[16];
			sprintf(buffer, "%02d:%02d", timerMinutes, timerSeconds);
			LCD_write_stringXY(1, 0, buffer);  // LCD에 현재 시간 표시
			FND_setFndNum((timerMinutes * 100) + timerSeconds); // FND에 시간 표시
		}
		break;
	}
}

void Fan_Run()
{
	// 팬 상태에 따라 모터 제어
	if (fanState == FAN_STATE_ON) {
		Gpio_writepin(&PORTE, 3, Gpio_pinset);    // 팬을 켬 (PE3 -> High)
	}
	else
	{
		Gpio_writepin(&PORTE, 3, Gpio_pinreset);  // 팬을 끔 (PE3 -> Low)
		
		// 타이머 모드일 때 LCD 클리어
		if (isTimerMode)
		{
			LCD_display_clear(); // 타이머 정보 제거
		}
	}

	// LCD 표시를 위한 코드
	if (isTimerMode)
	{
		char buffer[16];
		sprintf(buffer, "%02d:%02d", timerMinutes, timerSeconds);
		LCD_write_stringXY(1, 0, buffer);
	}
}
