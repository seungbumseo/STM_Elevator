#include "dcmotor.h"
#include "button.h"
extern TIM_HandleTypeDef htim1;
extern void led_all_off();
uint16_t CCR_value = 0;  // PWM control value
uint8_t pwm_start_flag = 0;  // PWM start flag
void dcmotor_pwm_control(void);
void show_led(uint16_t speed_value);
void dcmotor_pwm_control(void)
{
   // speed-up +10 (0.1ms HIGH)
   if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
   {
       CCR_value = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1);  // Read PWM setting
       CCR_value += 10;
       if (CCR_value > 100) CCR_value = 100;
       __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, CCR_value);
       show_led(CCR_value);
   }
   // speed-down -10 (0.1ms DOWN)
   if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
   {
       CCR_value = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1);  // Read PWM setting
       if (CCR_value >= 10)
       {
           CCR_value -= 10;
       }
       if (CCR_value < 60) CCR_value = 60;
       __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, CCR_value);
       show_led(CCR_value);
   }
   // pwm start -- pwm stop
   if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
   {
       if (!pwm_start_flag)
       {
           pwm_start_flag = 1;
           HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
       }
       else
       {
           pwm_start_flag = 0;
           HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
           led_all_off();
       }
   }
}
void show_led(uint16_t speed_value)
{
   if (!pwm_start_flag)
   {
       // If the motor is off, turn off all LEDs
       HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
       return;
   }
   int state = (speed_value - 60) / 10;
   switch (state)
   {
       case 0:
           HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
           HAL_GPIO_WritePin(GPIOB, 0xF0, GPIO_PIN_SET);
           break;
       case 1:
           HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
           HAL_GPIO_WritePin(GPIOB, 0xF8, GPIO_PIN_SET);
           break;
       case 2:
           HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
           HAL_GPIO_WritePin(GPIOB, 0xFC, GPIO_PIN_SET);
           break;
       case 3:
           HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
           HAL_GPIO_WritePin(GPIOB, 0xFE, GPIO_PIN_SET);
           break;
       case 4:
           HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
           HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_SET);
           break;
       default:
           HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
           HAL_GPIO_WritePin(GPIOB, 0x00, GPIO_PIN_RESET);
           break;
   }
}



