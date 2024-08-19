#include "uart2.h"

extern UART_HandleTypeDef huart2;
extern uint8_t rx_data;

#define COMMAND_MAX    20   // 최대로 저장 할 수 있는 command갯수
#define COMMAND_LENGTH  40  // command당 길이 40 byte
volatile int  rear=0;  // UART로 부터 들어온 문자를 저장 하는 2차원 index값
volatile int  front=0; // while(1) loop에서 가져 가는 index 값

volatile unsigned char rx_buff[COMMAND_MAX][COMMAND_LENGTH]; // 문자열을 저장하는 buffer

void pc_command_processing(void);

// move from HAL_UART_RxCpltCallback of stm32f4xx_hal_uart to here
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static volatile int i=0;

	if (huart == &huart2)
	{
		unsigned char data;

		data = rx_data;
		if (front % COMMAND_MAX==(rear+1) % COMMAND_MAX)
		{
			printf("Queue Full !!!\n");
		}
		else
		{
			if (data == '\n' || data == '\r')
			{
				rx_buff[rear][i]='\0';  // 문장의 끝을 \0로 만든다.
				i=0;
				rear++;
				rear %= COMMAND_MAX;
			}
			else
			{
				rx_buff[rear][i++]=data;
			}
		}

		HAL_UART_Receive_IT(&huart2, &rx_data, 1);   // 반드시 집어 넣어야 한다. assign RX INT
	}
}


void pc_command_processing(void)
{
	if (front != rear)  // rx_buff에 data가 들어 있으면
	{
		printf("rx_buff[%d]:%s\n", front, rx_buff[front]);
		if (strncmp((const char *)rx_buff[front],"settime",7)==NULL)
		{    //settime240705160400
             set_rtc(&rx_buff[front][7]);
		}
		front++;
		front %= COMMAND_MAX;
	}
}
