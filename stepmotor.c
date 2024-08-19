//#include "stepmotor.h"
//
//extern void delay_us(unsigned int us);
//void stepmotor_drive(int step);
//void set_rpm(int rpm);
//void stepmotor_main(void);
//void motor_control_loop(void);
//
///*
// * 	RPM(Revolutions Per Minutes) : 분당 회전수
// * 	1분 : 60sec : 1,000,000us (1초) * 60 = 60,000,000us
// * 	1초 : 1000ms --> 1ms(1000us) * 1000ms ==> 1,000,000us
// * 	1바퀴 회전 4096step 필요
// * 	4096 / 8  ---> 512 sequence : 360도 회전
// * 	1 sequence (8 step) : 0.70312도
// * 	0.70312도 * 512 sequence = 360 도
// * 	--- RPM 조정 함수 ---
// * 	60,000,000us / 4096 / rpm(1~13)
// * 	13 회전 : 60,000,000us / 4096 / 13 ==> step과 step 간격 time 1126us
// * 	1126us * 4096 (1회전시 필요 스텝) = 4,615,384 us
// * 								  = 4615ms
// * 								  = 4.6초
// * 	60초 / 4.6(1회전시 소요시간 초) ==> 13회 회전
// */
//typedef enum{
//	IDLE,
//	FORWARD,
//	REVERSE
//}MotorState;
//
//volatile MotorState current_state = IDLE;
//
//void set_rpm(int rpm)	//rpm : 1~13
//{
//	delay_us(60000000/4096/rpm);
//	// 최대 speed 기준 (13) : delay_us(1126);
//}
//
//void stepmotor_main(void)
//{
//		for(int i=0; i < 512; i++)	// 시계방향 1바퀴 회전
//		{
//			for(int j=0; j < 8; j++)
//			{
//				stepmotor_drive(j);
//				set_rpm(13);	//1126 us만큼 wait
//			}
//		}
//
//		for(int i=0; i < 512; i++)	// 시계방향 1바퀴 회전
//				{
//					for(int j=7; j >=0; j--)
//					{
//						stepmotor_drive(j);
//						set_rpm(13);
//					}
//				}
//	}
//
//
//
//void stepmotor_drive(int step)
//{
//	switch (step)
//	{
//	case 0:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
//		break;
//	case 1:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
//		break;
//	case 2:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
//		break;
//	case 3:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
//		break;
//	case 4:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
//		break;
//	case 5:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
//		break;
//	case 6:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
//		break;
//	case 7:
//		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
//		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
//		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
//		break;
//	}
//}
//
//void motor_control_loop(void) {
//    static int forward_step = 0;
//    static int reverse_step = 7;
//    static int sequence_count = 0;
//
//    switch (current_state) {
//          case FORWARD:
//              stepmotor_drive(forward_step);
//              set_rpm(13);
//              forward_step++;
//              if (forward_step >= 8) {
//                  forward_step = 0;
//                  sequence_count++;
//                  if (sequence_count >= 512) {
//                      sequence_count = 0; // Reset sequence count after one full rotation
//                  }
//              }
//              break;
//
//          case REVERSE:
//              stepmotor_drive(reverse_step);
//              set_rpm(13);
//              reverse_step--;
//              if (reverse_step < 0) {
//                  reverse_step = 7;
//                  sequence_count++;
//                  if (sequence_count >= 512) {
//                      sequence_count = 0; // Reset sequence count after one full rotation
//                  }
//              }
//              break;
//
//          case IDLE:
//          default:
//              sequence_count = 0; // Reset sequence count when idle
//              break;
//      }
//  }

