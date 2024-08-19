#include "stm32f4xx_hal.h"
#include "button.h"
#include "photo_int.h"
#include "stepmotor.h"
#include "i2c_lcd.h"
#include "dotmatrix.h"

extern TIM_HandleTypeDef htim3;
extern void led_all_off();
extern void led_all_on();
extern void init_dotmatrix(void);
extern void update_scroll_buffer(int floor, int direction);
extern void dotmatrix_main(void);
extern void update_dotmatrix_scroll(int direction);
extern void dotmatrix_main_test();
volatile int direction = 0;



typedef enum {
    FLOOR1 = 1,
    FLOOR2,
    FLOOR3,
    FLOOR4,
    IDLE
} MotorState;

volatile MotorState current_state = FLOOR1; // 시작 층은 1층
volatile MotorState target_state = IDLE;
volatile int nowfloor = 1;
volatile int flag = 0;
volatile int targetfloor=0;

unsigned char button_status[BUTTON_NUMBER] = {BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);

TIM_HandleTypeDef htim3;

void button0_check(void) {
    if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS) {
        target_state = FLOOR1;
        update_lcd2();

    }
}

void button1_check(void) {
    if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS) {
        target_state = FLOOR2;
        update_lcd2();


    }
}

void button2_check(void) {
    if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS) {
        target_state = FLOOR3;
        update_lcd2();


    }
}

void button3_check(void) {
    if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS) {
        target_state = FLOOR4;
        update_lcd2();


    }
}

/*void button_check(void) {
    if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS) {
        target_state = FLOOR1;
        update_lcd2();
        led_all_off();
        HAL_GPIO_WritePin(GPIOB, 0x01, 1);
    } else if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS) {
        target_state = FLOOR2;
        update_lcd2();
        led_all_off();
        HAL_GPIO_WritePin(GPIOB, 0x03, 1);
    } else if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS) {
        target_state = FLOOR3;
        update_lcd2();
        led_all_off();
        HAL_GPIO_WritePin(GPIOB, 0x07, 1);
    } else if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS) {
        target_state = FLOOR4;
        update_lcd2();
        led_all_off();
        HAL_GPIO_WritePin(GPIOB, 0x0F, 1);
    }
}*/


int get_button(GPIO_TypeDef *GPIO, int button_pin, int button_num) {
    int current_state;
    current_state = HAL_GPIO_ReadPin(GPIO, button_pin);
    if (current_state == BUTTON_PRESS && button_status[button_num] == BUTTON_RELEASE) {
        HAL_Delay(60);
        button_status[button_num] = BUTTON_PRESS;
        return BUTTON_RELEASE;
    } else if (button_status[button_num] == BUTTON_PRESS && current_state == BUTTON_RELEASE) {
        button_status[button_num] = BUTTON_RELEASE;
        HAL_Delay(60);
        return BUTTON_PRESS;
    }
    return BUTTON_RELEASE;
}

void set_rpm(int rpm) {
    delay_us(60000000 / 4096 / rpm);
}

void stepmotor_drive(int step) {
    switch (step) {
        case 0:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
            HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
            HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
            break;
    }
}

void stepmotor_stop(void) {
    HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
}

void motor_control_loop(void)
{
    static int step = 0;

    if ((flag > 0) && (nowfloor == target_state)) {
        stepmotor_stop();
        direction = 0;
        current_state = target_state;
        target_state = IDLE;
        flag = 0;

        //문 여닫기 코드
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 50);
        HAL_Delay(500);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 100);

        return;
    }

    if (target_state != IDLE) {
        if (target_state > nowfloor) {
        	direction = 1;
            stepmotor_drive(step % 8);
            set_rpm(13);
            step++;
        } else if (target_state < nowfloor) {
        	direction = -1;
            stepmotor_drive(step % 8);
            set_rpm(13);
            step--;
        }

    }

}

void update_lcd(void) {
    char buffer[16];
    move_cursor(0, 0);
    sprintf(buffer, "FLOOR_NOW: %d", nowfloor);
    lcd_string((uint8_t *)buffer);

}

void update_lcd2(void) {
    char buffer[16];
    move_cursor(1, 0);
    sprintf(buffer, "FLOOR_GOING: %d", target_state == IDLE ? 0 : target_state);
    lcd_string((uint8_t *)buffer);
}
//interrupt-callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    switch (GPIO_Pin) {
        case GPIO_PIN_0:
            nowfloor = 1;
            flag = 1;
            update_lcd();
            break;
        case GPIO_PIN_1:
            nowfloor = 2;
            flag = 2;
            update_lcd();
            break;
        case GPIO_PIN_5:
            nowfloor = 3;
            flag = 3;
            update_lcd();
            printf("3\n");
            break;
        case GPIO_PIN_6:
            nowfloor = 4;
            flag = 4;
            update_lcd();

            break;
    }

}

