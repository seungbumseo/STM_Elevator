#include "servo_motor.h"

void servo_motor_main(void);

extern TIM_HandleTypeDef htim3;
extern volatile int TIM2_servo_motor_counter;
/*
동작 주파수 : 84000000HZ
TIM3 50KHZ 입력 : 84000000/1680 ==> 50,000HZ(50KHZ)
T=1/f = 1/50000 = 0.00002sec(20us)
20ms : 0.00002 x 1000개
2ms(180도) : 0.00002 x 100개
1.5ms(90도) : 0.00002 x 75개
1ms(0도) : 0.00002 x 50개
 */

void servo_motor_main(void)
{
	static uint8_t servo_state=0;

	if (TIM2_servo_motor_counter >= 1000)  // 1sec
	{
		TIM2_servo_motor_counter=0;
		// 180 --> 90 --> 0
		switch(servo_state)
		{
		case 0:
			// 1. 180도 회전
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 100);
			break;
		case 1:
			// 2. 90도 회전
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 75);
			break;
		case 2:
			// 3. 0도 회전
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 50);
			break;
		}
		servo_state++;
		servo_state %= 3;
	}
}





