#include "protocol.h"

#define PROTOCOL_HEADER1 0xAA
#define PROTOCOL_HEADER2 0x55
#define PROTOCOL_MAX_DATA 64
/* 状态机枚举 */
typedef enum e_ProtocolState
{
    STATE_HEAD1 = 0,
    STATE_HEAD2,
    STATE_ADDR,
    STATE_CMD,
    STATE_LEN,
    STATE_DATA,
    STATE_CHECKSUM
} ProtocolState_t;
//* 数据帧结构体 */
typedef struct s_ProtocolFrame
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
    uint8_t data[PROTOCOL_MAX_DATA];
} ProtocolFrame_t;
