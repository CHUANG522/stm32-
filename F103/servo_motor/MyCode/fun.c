#include "fun.h"

void function(void)
{
	OLED_show();
}
uint8_t OLED_buff[20];
uint32_t time_OLED;
uint32_t time;
void OLED_show(void)
{
	if (time_OLED < 100)
		return;
	else
		time_OLED = 0;

	OLED_Clear();
	sprintf((char *)OLED_buff, "hello world!");
	OLED_ShowString(1, 1, (char *)OLED_buff);
	sprintf((char *)OLED_buff, "CCR3:%d", TIM2->CCR3);
	OLED_ShowString(2, 1, (char *)OLED_buff);
}
void set_servo_angle(float angle)
{
	if (angle < 0.0f)
		angle = 0.0f;
	else if (angle > 180.0f)
		angle = 180.0f;
	else
	{
		uint16_t temp = 11.11f * angle + 500;
		//	TIM2->CCR3 = temp;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, temp);
	}
}
