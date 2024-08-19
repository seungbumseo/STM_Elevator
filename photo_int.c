#include "photo_int.h"
#include "motor_state.h"
//extern volatile MotorState current_state;
extern void led_all_on(void);
extern void led_all_off(void);
// move from Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c to here
// external interrupt call back function
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
// switch(GPIO_Pin)
// {
// case GPIO_PIN_0: 	//PA.0
//printf("GPIO_PIN_0\n");
//
//HAL_GPIO_WritePin(GPIOB, 0x01, 1);
//            current_state = FORWARD;
//	 break;
// case GPIO_PIN_1: 	//PA.1
//printf("GPIO_PIN_1\n");
//
//HAL_GPIO_WritePin(GPIOB, 0x02, 1);
//            current_state = REVERSE;
//
//	 break;
// }
//}


