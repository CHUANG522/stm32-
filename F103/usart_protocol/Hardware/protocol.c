#include "protocol.h"

#define PROTOCOL_HEADER1 (0xAA)
#define PROTOCOL_HEADER2 (0x55)
#define PROTOCOL_MAX_DATA (64)
/* 状态机枚举 */
typedef enum e_ProtocolState
{
    STATE_HEAD1 = 0,
    STATE_HEAD2,
    STATE_ADDR,
    STATE_CMD,
    STATE_LEN,
    STATE_DATA,
    STATE_CRC_L, // 接收 CRC 低字节
    STATE_CRC_H  // 接收 CRC 高字节

} ProtocolState_t;
//* 数据帧结构体 */
typedef struct s_ProtocolFrame
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
    uint8_t data[PROTOCOL_MAX_DATA];
} ProtocolFrame_t;
/*解析器控制块*/
typedef struct s_ProtocolParser
{
    ProtocolState_t state;
    ProtocolFrame_t frame;
    uint16_t calc_crc; // 保存实时计算的crc值
    uint8_t data_index;

} ProtocolParser_t;
/**
 * @brief  初始化/复位解析器状态
 */
void Protocol_Init(ProtocolParser_t *parser)
{
    if (parser == NULL)
        return;
    parser->state = STATE_HEAD1;
    parser->data_index = 0;
    parser->calc_crc = 0XFFFF; // Modbus CRC 初始值必须是 0xFFFF
    memset(&parser->frame, 0, sizeof(ProtocolFrame_t));
}
/**
 * @brief  计算 CRC16-Modbus 校验码
 * @param  *data : 指向要发送或接收的数据数组的指针
 * @param  length: 数据的长度（字节数）
 * @retval 16位的 CRC 校验码 (纯数学结果)
 */

uint16_t Calculate_CRC16_Modbus(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF; // 1. 寄存器初始值设为 0xFFFF

    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= data[i]; // 2. 将数据字节与 CRC 寄存器的低8位进行异或运算

        for (uint8_t j = 0; j < 8; j++) // 3. 对每个字节循环 8 次处理
        {
            if (crc & 0x0001) // 4. 判断最低位是否为 1
            {
                crc >>= 1;     // 如果是 1，寄存器右移 1 位
                crc ^= 0xA001; // 并与多项式 0xA001 进行异或运算
            }
            else
            {
                crc >>= 1; // 如果是 0，只进行右移操作
            }
        }
    }

    return crc; // 返回计算出来的 16 位纯数学结果
}