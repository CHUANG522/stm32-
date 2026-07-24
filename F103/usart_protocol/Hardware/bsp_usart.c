#include "bsp_usart.h"
#define string_max_len 256 // 单次发送的最大字节数
// 1. 定义接收相关的全局缓冲区
#define RX_BUF_SIZE 256         // 单次接收的最大字节数
uint8_t rx_buffer[RX_BUF_SIZE]; // DMA 硬件直接搬运的原始温床
uint8_t rx_data_flag = 0;       // 接收完成标志位：1 代表收到了一帧完整数据
uint16_t rx_data_len = 0;       // 记录收到了多少个字节

void send_string(uint8_t *string)
{
    if (string == NULL)
        return;
    uint16_t string_len = strlen((char *)string);
    if (string_len > string_max_len)
        string_len = string_max_len;

    uint16_t time_out = (string_len + 1) * 2;
    HAL_UART_Transmit(&huart1, string, string_len, time_out);
}
void send_string_dma(uint8_t *string)
{
    if (string == NULL)
        return;
    uint16_t len = strlen((char *)string);
    if (len == 0)
        return;

    HAL_UART_Transmit_DMA(&huart1, string, len);
}
void USART1_DMA_Rx_Start(void)
{
    // 使用 ReceiveToIdle_DMA 实现不定长数据接收
    // DMA 自动搬运数据到 rx_buffer，当串口空闲（一帧结束）时触发回调
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, RX_BUF_SIZE);
}
void USART1_Process_Data(void)
{
    if (rx_data_flag == 1)
    {
        // 处理数据：用阻塞式发送回显，确保发送完毕再清零缓冲区
        // 不能用 send_string_DMA 发送，因为 send_string_dma 是异步的，CPU在告诉DMA后就执行了清除rx_buffer的命令
        // 阻塞发送send_string
        send_string(rx_buffer);
        // 处理完后，清空缓冲区并复位标志位，等待下一帧
        memset(rx_buffer, 0, RX_BUF_SIZE);
        rx_data_len = 0;
        rx_data_flag = 0;

        // 重新开启 DMA 接收，准备接收下一帧
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, RX_BUF_SIZE);
    }
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        // 1. 停止当前的 DMA 接收，防止在处理数据期间有新数据冲刷缓冲区
        HAL_UART_DMAStop(huart);

        // 2. 记录数据长度并立起小红旗（标志位）
        rx_data_len = Size;
        rx_data_flag = 1;
    }
}
