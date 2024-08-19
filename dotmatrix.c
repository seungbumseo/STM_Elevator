#include "dotmatrix.h"
#include "elevator.h"

extern SPI_HandleTypeDef hspi2;
void dotmatrix_main_test();
int dotmatrix_main(void);
void init_dotmatrix(void);
volatile int TIM2_DOT_counter;


extern volatile current_state; // 시작 층은 1층
extern volatile target_state;
extern volatile int nowfloor;
extern volatile int flag;
extern volatile int direction;




uint8_t allon[] = {			// allon 문자 정의
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111
};


uint8_t smile[] = {			// 스마일 문자 정의
	0b00111100,
	0b01000010,
	0b10010101,
	0b10100001,
	0b10100001,
	0b10010101,
	0b01000010,
	0b00111100 };

uint8_t hart[] = {		// hart
	0b00000000,    // hart
	0b01100110,
	0b11111111,
	0b11111111,
	0b11111111,
	0b01111110,
	0b00111100,
	0b00011000
};

uint8_t one[] =
{0b00011000,
0b00111000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b01111110,
0b01111110};


uint8_t col[4]={0,0,0,0};

void dotmatrix_main_test()
{
  dotmatrix_main();

  while (1)
  {
        for (int i=0; i < 8; i++)
        {
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = hart[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
        }
  }
}

uint8_t up_arrow[] = {
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000
};

uint8_t down_arrow[] = {
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000
};

uint8_t number_data_1[8] = {
    0b01000000, // 1
    0b11000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b11100000,
    0b00000110 // 점 0b00000110
};

// 숫자 2의 데이터
uint8_t number_data_2[8] = {
    0b01110000, // 2
    0b10001000,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b11111000,
    0b00000110 // 점 0b00000110
};

// 숫자 3의 데이터
uint8_t number_data_3[8] = {
    0b11111000, // 3
    0b00010000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b10001000,
    0b01110000,
    0b00000110 // 점 0b00000110
};

// 숫자 4의 데이터
uint8_t number_data_4[8] = {
    0b00010000, // 4
    0b00110000,
    0b01010000,
    0b10010000,
    0b11111000,
    0b00010000,
    0b00010000,
    0b00000110 // 점 0b00000110
};

uint8_t el_num[20][10] =
{
		{0x00,0x08,0x18,0x08,0x08,0x08,0x1c,0x00}, // 1
		{0x00,0x18,0x24,0x04,0x08,0x10,0x3c,0x00}, // 2
		{0x00,0x00,0x3c,0x04,0x3c,0x04,0x3c,0x00}, // 3
		{0x00,0x08,0x18,0x28,0x7c,0x08,0x08,0x00}  // 4
};

uint8_t el_ud[20][30] =
{
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x1c,0x3e,0x1c,0x1c,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	//up
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x1c,0x1c,0x3e,0x1c,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}	//down
};

unsigned char display_data[8];  // 최종 8x8 출력할 데이터
unsigned char scroll_buffer[50][8] = {0};  // 스코롤할 데이타가 들어있는 버퍼
int number_of_character = 11;  // 출력할 문자 갯수

// 초기화 작업
// 1. display_data에 number_data[0]에 있는 내용 복사
// 2. number_data를 scroll_buffer에 복사
// 3. dotmatrix의 led를 off
//void init_dotmatrix(void)
//{
//
//
//	for (int i=0; i < 8; i++)
//	{
//		display_data[i] = number_data[i];
//	}
//	for (int i=1; i < number_of_character+1; i++)
//	{
//		for (int j=0; j < 8; j++) // scroll_buffer[0] = blank
//		{
//			scroll_buffer[i][j] = number_data[i-1][j];
//		}
//	}
//}
void init_dotmatrix(void)
{
	for (int i=0; i < 8; i++)
	{
		display_data[i] = allon[i]; // 변경: number_data 대신에 allon 사용
	}
	for (int i=1; i < number_of_character+1; i++)
	{
		for (int j=0; j < 8; j++) // scroll_buffer[0] = blank
		{
			switch(i) {
				case 1:
					scroll_buffer[i][j] = number_data_1[j]; // 변경: number_data 대신에 number_data_1 사용
					break;
				case 2:
					scroll_buffer[i][j] = number_data_2[j]; // 변경: number_data 대신에 number_data_2 사용
					break;
				case 3:
					scroll_buffer[i][j] = number_data_3[j]; // 변경: number_data 대신에 number_data_3 사용
					break;
				case 4:
					scroll_buffer[i][j] = number_data_4[j]; // 변경: number_data 대신에 number_data_4 사용
					break;
				default:
					scroll_buffer[i][j] = 0; // 기본 값
					break;
			}
		}
	}
}
// scroll 문자 출력 프로그램
int dotmatrix_main(void)
{
	static int count=0;  // 컬럼 count
	static int index=0;  // scroll_buffer의 2차원 index값
	static uint32_t past_time=0;  // 이전 tick값 저장
	 static int direction = 0;  // 방향 (1: 상승, -1: 하강, 0: 멈춤)
	 static int floor = 0;  // 현재 층수

	init_dotmatrix();

	while(1)
	{
		uint32_t now = HAL_GetTick();  // 1ms
		// 1.처음시작시 past_time=0; now: 500 --> past_time=500

		if (now - past_time >= 500) // 500ms scroll
		{
			past_time = now;
			for (int i=0; i < 8; i++)
			{

				display_data[i] = (scroll_buffer[index][i] >> count) |
						(scroll_buffer[index+1][i] << 8 - count);
			}
			if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
			{
				count =0;
				index++;  // 다음 scroll_buffer로 이동
				if (index == number_of_character+1) index=0;
				// 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
			}
		}
		for (int i=0; i < 8; i++)
		{
			// 공통 양극 방식
			// column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = display_data[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
		}
	}
	return 0;
}



//void update_scroll_buffer(int floor, int direction) {
//    uint8_t* number_data;
//
//    // 현재 층수에 해당하는 숫자 데이터 선택
//    switch(floor) {
//        case 1:
//            number_data = number_data_1;
//            break;
//        case 2:
//            number_data = number_data_2;
//            break;
//        case 3:
//            number_data = number_data_3;
//            break;
//        case 4:
//            number_data = number_data_4;
//            break;
//        default:
//            number_data = number_data_1; // 기본 값
//            break;
//    }
//
//    for (int i = 0; i < 8; i++) {
//        if (direction > 0) {
//            scroll_buffer[0][i] = number_data[i]; // 현재 층수
//            scroll_buffer[1][i] = up_arrow[i];    // 상승 화살표
//        } else if (direction < 0) {
//            scroll_buffer[0][i] = number_data[i]; // 현재 층수
//            scroll_buffer[1][i] = down_arrow[i];  // 하강 화살표
//        } else {
//            scroll_buffer[0][i] = number_data[i]; // 현재 층수
//        }
//    }
//}


void elavator_dot(void)
{
	static int m=0;
	static int i=0;
	static int j=0;
	static int k=0;
	static int l=0;
	switch (direction)
	{
	case 0:		// stop
		col[0] = ~(1 << m);		// i=0일때 = 11111110    == 8번째 col만 0 돼서 켜짐
		col[1] = el_num[current_state][m];
		HAL_SPI_Transmit(&hspi2, col, 2, 1);	//	2 = 2바이트, 1 = 타임아웃(1ms이내에 전송한다)
		GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
		GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
		HAL_Delay(1);
		m++;
		m%=8;
		break;
	case 1:		// up
		if(j<17)
		{
			col[0] = ~(1 << i);		// i=0일때 = 11111110    == 8번째 col만 0 돼서 켜짐
			col[1] = el_ud[0][i+j];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);	//	2 = 2바이트, 1 = 타임아웃(1ms이내에 전송한다)
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
			i++;
			i%=8;
		}
		else
		{
			col[0] = ~(1 << i);		// i=0일때 = 11111110    == 8번째 col만 0 돼서 켜짐
			col[1] = el_num[current_state][i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);	//	2 = 2바이트, 1 = 타임아웃(1ms이내에 전송한다)
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
			i++;
			i%=8;
		}
		if(TIM2_DOT_counter>=150)
		{
			TIM2_DOT_counter=0;
			j++;
			j%=25;
		}
		break;
	case -1:		// down
		if(k<17)
		{
			col[0] = ~(1 << l);		// i=0일때 = 11111110    == 8번째 col만 0 돼서 켜짐
			col[1] = el_ud[1][15+l-k];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);	//	2 = 2바이트, 1 = 타임아웃(1ms이내에 전송한다)
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
			l++;
			l%=8;
		}
		else
		{
			col[0] = ~(1 << l);		// i=0일때 = 11111110    == 8번째 col만 0 돼서 켜짐
			col[1] = el_num[current_state-1][l];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);	//	2 = 2바이트, 1 = 타임아웃(1ms이내에 전송한다)
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
			l++;
			l%=8;
		}
		if(TIM2_DOT_counter>=150)
		{
			TIM2_DOT_counter=0;
			k++;
			k%=25;
		}
		break;
	case 3:
		break;
	}
}



