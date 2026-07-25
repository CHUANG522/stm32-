
#pragma once

#include "Header.h"
void send_string(uint8_t *string);
void send_string_dma(uint8_t *string);
void USART1_Process_Data(void);
void USART1_DMA_Rx_Start(void);

extern uint8_t rx_buffer[];
