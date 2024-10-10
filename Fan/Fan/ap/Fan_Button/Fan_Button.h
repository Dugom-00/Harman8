#ifndef FAN_BUTTON_H_
#define FAN_BUTTON_H_

#include "../../Driver/Button/Button.h" // 버튼 관련 헤더 파일 경로
#include "../../Periph/GPIO/GPIO.h"		// GPIO 관련 헤더 파일 경로
#include "../../Periph/TIM/TIM.h"		// TIM 관련 헤더 파일 경로
#include "../../Driver/LCD/LCD.h"		// LCD 관련 헤더 파일 경로
#include "../../Driver/FND/FND.h"		// FND 관련 헤더 파일 경로

// 팬 상태 정의
#define FAN_STATE_OFF 0
#define FAN_STATE_ON  1
#define FAN_STATE_TIMER_MODE 2

// 팬 속도 모드 정의
#define FAN_MODE1 1  // 선풍기 속도 1 (MODE1)
#define FAN_MODE2 2  // 선풍기 속도 2 (MODE2)
#define FAN_MODE3 3  // 선풍기 속도 3 (MODE3)



// 버튼 초기화 함수
void Fan_Button_Init(void);

// 버튼 이벤트를 처리하는 함수
void Fan_EventCheck(void);

// 팬 동작을 실행하는 함수
void Fan_Run(void);

// 메인 루프에서 호출할 함수
void Fan_Button_Execute(void);

// 팬 속도를 설정하는 함수
void Set_Fan_Speed(uint8_t speed);

// 모드 변경 시 LCD 업데이트 함수
void Update_LCD_Mode(void);

// 타이머 업데이트 함수
void Update_LCD_Timer(void);

// Min 계산하는 함수
void TimeClock_incMin();

// Hour 계산하는 함수
void TimeClock_incHour();

#endif /* FAN_BUTTON_H_ */
