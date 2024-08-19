#include "main.h"    // GPIO/HAL정보
#include <string.h>  // for strncpy etc
#include <stdlib.h>  // atoi itoa etc

void get_rtc_date_time(void);
unsigned char bcd2dec(unsigned char byte);
unsigned char dec2bcd(unsigned char byte);
void set_rtc(char *date_time);

extern RTC_HandleTypeDef hrtc;
extern void lcd_string(uint8_t *str);
extern void move_cursor(uint8_t row, uint8_t column);

RTC_TimeTypeDef sTime = {0};   // 시각 정보
RTC_DateTypeDef sDate = {0};   // 날짜 정보
// uint8_t Year;
// 예) 24년의 Year에 저장된 data format
// 7654 3210
// ---- ----
// 0010 0100
// 0000 0010 * 10
//   2   4
// ==> 24
unsigned char bcd2dec(unsigned char byte)
{
	unsigned char high, low;

	low = byte & 0x0f;   // 상위 4bit는 무시하고 하위 4bit만 취함.
	high = (byte >> 4) * 10;

	return (high+low);

}
// 10진수를 BCD format으로 변환
// 10진수 예) 24(00011000) ===> 0010 0100
unsigned char dec2bcd(unsigned char byte)
{
	unsigned char high, low;

	high = (byte / 10) << 4;
	low = byte % 10;

	return (high+low);

}
// RTC에서 날짜와 시각정보를 읽어 오는 함수
void get_rtc_date_time(void)
{
	static RTC_TimeTypeDef oldTime;
	char lcdbuff[40];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	if (oldTime.Seconds != sTime.Seconds)
	{
		// 현재 update된 정보를 출력 (1초에 1번씩 출력)
		// YYYY-MM-DD HH:mm:ss
		printf("%04d-%02d-%02d %02d:%02d:%02d\n",
				bcd2dec(sDate.Year)+2000,bcd2dec(sDate.Month),bcd2dec(sDate.Date),
				bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes),bcd2dec(sTime.Seconds));
		sprintf(lcdbuff, "%04d-%02d-%02d",
				bcd2dec(sDate.Year)+2000,bcd2dec(sDate.Month),bcd2dec(sDate.Date));
		move_cursor(0, 0);
		lcd_string(lcdbuff);

		sprintf(lcdbuff, "%02d:%02d:%02d",
				bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes),bcd2dec(sTime.Seconds));
		move_cursor(1, 0);
		lcd_string(lcdbuff);

		oldTime.Seconds = sTime.Seconds;
	}
}

// 시각 보정 기능
// setrtc240524141800  (24년05월24일 14시18분 00초 )
// 240524141800를 넘겨 받는다. call by referece
void set_rtc(char *date_time)
{
	char yy[4], mm[4], dd[4];  // date
	char hh[4], min[4], ss[4];  // time정보

	strncpy(yy, date_time,2);
	strncpy(mm, date_time+2,2);  // date_time+2 == &date_time[2]
	strncpy(dd, date_time+4,2);

	strncpy(hh, date_time+6,2);
	strncpy(min, date_time+8,2);
	strncpy(ss, date_time+10,2);
	// 1. ascii --> int 2. int --> bcd 3. RTC에 적용
	sDate.Year = dec2bcd(atoi(yy));
	sDate.Month = dec2bcd(atoi(mm));
	sDate.Date = dec2bcd(atoi(dd));

	sTime.Hours = dec2bcd(atoi(hh));
	sTime.Minutes = dec2bcd(atoi(min));
	sTime.Seconds = dec2bcd(atoi(ss));

	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
}










