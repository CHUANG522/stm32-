#pragma once

#include "Header.h"

void function(void); // 此函数在main中循环
void OLED_show(void);
void USART_show(void);
// 全局变量
extern uint32_t time_OLED;
extern uint32_t time_USART;


